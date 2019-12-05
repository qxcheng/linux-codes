import os
import numpy as np
import torch
from PIL import Image
import torchvision
from torchvision.models.detection.faster_rcnn import FastRCNNPredictor
from torchvision.models.detection.mask_rcnn import MaskRCNNPredictor
from engine import train_one_epoch, evaluate
import utils
import transforms as T
np.set_printoptions(threshold=np.inf)


class PennFudanDataset(object):
    def __init__(self, root, transforms):
        self.root = root
        self.transforms = transforms
        self.imgs = list(sorted(os.listdir(os.path.join(root, "PNGImages")))) # ['FudanPed00001.png', 'FudanPed00002.png', ...]
        self.masks = list(sorted(os.listdir(os.path.join(root, "PedMasks")))) # ['FudanPed00001_mask.png', 'FudanPed00002_mask.png', ...]

    def __getitem__(self, idx):
        img_path = os.path.join(self.root, "PNGImages", self.imgs[idx])
        mask_path = os.path.join(self.root, "PedMasks", self.masks[idx])
        img = Image.open(img_path).convert("RGB")  #(h, w, 3)
        mask = Image.open(mask_path)               #(h, w)    0:background 1:instance1 2:instance2...
        
        mask = np.array(mask)                  #(h, w) 
        obj_ids = np.unique(mask)              #[0, 1, 2] 代表有两个实例，可能为多个
        obj_ids = obj_ids[1:]                  #0 is the background, so remove it
        masks = mask == obj_ids[:, None, None] #[2, h, w] 这里有两个，可能多个，每个位置值为bool值，代表是否是实例部分

        num_objs = len(obj_ids)                    # 这里为2个实例   
        boxes = []
        for i in range(num_objs):
            pos = np.where(masks[i])               # 第i个实例掩码True的坐标，两个元组形式给出
            xmin = np.min(pos[1])                  # x坐标对应w下标值
            xmax = np.max(pos[1])
            ymin = np.min(pos[0])                  # y坐标对应h下标值
            ymax = np.max(pos[0])
            boxes.append([xmin, ymin, xmax, ymax]) # 矩形目标检测框的坐标

        boxes = torch.as_tensor(boxes, dtype=torch.float32)  # 这里2个矩形目标检测框
        labels = torch.ones((num_objs,), dtype=torch.int64)  # there is only one class
        masks = torch.as_tensor(masks, dtype=torch.uint8)    # [2, h, w]

        image_id = torch.tensor([idx])                                   # 图片id
        area = (boxes[:, 3] - boxes[:, 1]) * (boxes[:, 2] - boxes[:, 0]) # 面积
        iscrowd = torch.zeros((num_objs,), dtype=torch.int64)            # suppose all instances are not crowd

        target = {}
        target["boxes"] = boxes
        target["labels"] = labels
        target["masks"] = masks
        target["image_id"] = image_id
        target["area"] = area
        target["iscrowd"] = iscrowd

        if self.transforms is not None:
            img, target = self.transforms(img, target)

        return img, target

    def __len__(self):
        return len(self.imgs)

def get_model_instance_segmentation(num_classes):
    model = torchvision.models.detection.maskrcnn_resnet50_fpn(pretrained=True)

    in_features = model.roi_heads.box_predictor.cls_score.in_features           # 1024
    model.roi_heads.box_predictor = FastRCNNPredictor(in_features, num_classes) 

    in_features_mask = model.roi_heads.mask_predictor.conv5_mask.in_channels    # 256
    hidden_layer = 256
    model.roi_heads.mask_predictor = MaskRCNNPredictor(in_features_mask, hidden_layer, num_classes) 

    return model
'''(roi_heads): RoIHeads(
        (box_roi_pool): MultiScaleRoIAlign()
        (box_head): TwoMLPHead(
        (fc6): Linear(in_features=12544, out_features=1024, bias=True)
        (fc7): Linear(in_features=1024, out_features=1024, bias=True)
        )
        (box_predictor): FastRCNNPredictor(
        (cls_score): Linear(in_features=1024, out_features=91, bias=True)
        (bbox_pred): Linear(in_features=1024, out_features=364, bias=True)
        )
        (mask_roi_pool): MultiScaleRoIAlign()
        (mask_head): MaskRCNNHeads(
        (mask_fcn1): Conv2d(256, 256, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1))
        (relu1): ReLU(inplace)
        (mask_fcn2): Conv2d(256, 256, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1))
        (relu2): ReLU(inplace)
        (mask_fcn3): Conv2d(256, 256, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1))
        (relu3): ReLU(inplace)
        (mask_fcn4): Conv2d(256, 256, kernel_size=(3, 3), stride=(1, 1), padding=(1, 1))
        (relu4): ReLU(inplace)
        )
        (mask_predictor): MaskRCNNPredictor(
        (conv5_mask): ConvTranspose2d(256, 256, kernel_size=(2, 2), stride=(2, 2))
        (relu): ReLU(inplace)
        (mask_fcn_logits): Conv2d(256, 91, kernel_size=(1, 1), stride=(1, 1))
    )'''


def get_transform(train):
    transforms = []
    transforms.append(T.ToTensor())
    if train:
        transforms.append(T.RandomHorizontalFlip(0.5))
    return T.Compose(transforms)


def train():
    device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
    num_classes = 2   # background and person
    dataset = PennFudanDataset('PennFudanPed', get_transform(train=True))
    dataset_test = PennFudanDataset('PennFudanPed', get_transform(train=False))

    # split the dataset in train and test set
    indices = torch.randperm(len(dataset)).tolist()
    dataset = torch.utils.data.Subset(dataset, indices[:-50])
    dataset_test = torch.utils.data.Subset(dataset_test, indices[-50:])

    # define training and validation data loaders
    data_loader = torch.utils.data.DataLoader(dataset, batch_size=2, shuffle=True, num_workers=4, collate_fn=utils.collate_fn)
    data_loader_test = torch.utils.data.DataLoader(dataset_test, batch_size=1, shuffle=False, num_workers=4, collate_fn=utils.collate_fn)

    model = get_model_instance_segmentation(num_classes)
    model.to(device)
    params = [p for p in model.parameters() if p.requires_grad]
    optimizer = torch.optim.SGD(params, lr=0.005, momentum=0.9, weight_decay=0.0005)
    lr_scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=3, gamma=0.1)

    num_epochs = 10
    for epoch in range(num_epochs):
        # train for one epoch, printing every 10 iterations
        train_one_epoch(model, optimizer, data_loader, device, epoch, print_freq=10)
        lr_scheduler.step()                              # update the learning rate
        #evaluate(model, data_loader_test, device=device) # evaluate on the test dataset

        torch.save(model.state_dict(), './checkpoints/maskrcnn_pretrained.pth')
    
    print("That's it!")

def predict():
    device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
    model = get_model_instance_segmentation(2)
    model.to(device)
    model.load_state_dict(torch.load('./checkpoints/maskrcnn_pretrained.pth', map_location='cpu'))
    model.eval()
    import torchvision.transforms as TT
    transform = TT.Compose([TT.ToTensor()])
    img = Image.open('./23.png')
    img = transform(img)
    predictions = model([img])
    print(predictions)

    
if __name__ == "__main__":
    #dataset = PennFudanDataset('PennFudanPed', get_transform(train=False))
    #dataset[1]

    #train()
    predict()

    

