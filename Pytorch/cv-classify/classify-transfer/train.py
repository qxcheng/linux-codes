# -*- coding: utf-8 -*-
from __future__ import print_function, division
import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import lr_scheduler
import torchvision
from torchvision import datasets, models, transforms
import numpy as np
import matplotlib.pyplot as plt
import time
import os
import copy

# visualize training images 
def imshow(inp, title=None):
    """Imshow for Tensor."""
    inp = inp.numpy().transpose((1, 2, 0))
    mean = np.array([0.485, 0.456, 0.406])
    std = np.array([0.229, 0.224, 0.225])
    inp = std * inp + mean
    inp = np.clip(inp, 0, 1)
    plt.imshow(inp)
    if title is not None:
        plt.title(title)
    plt.pause(0.001)  # pause a bit so that plots are updated


# Visualizing the model predictions
def visualize_model(model, num_images=6):
    was_training = model.training
    model.eval()
    images_so_far = 0
    fig = plt.figure()

    with torch.no_grad():
        for i, (inputs, labels) in enumerate(dataloaders['val']):
            inputs = inputs.to(device)
            labels = labels.to(device)

            outputs = model(inputs)
            _, preds = torch.max(outputs, 1)

            for j in range(inputs.size()[0]):
                images_so_far += 1
                ax = plt.subplot(num_images//2, 2, images_so_far)
                ax.axis('off')
                ax.set_title('predicted: {}'.format(class_names[preds[j]]))
                imshow(inputs.cpu().data[j])

                if images_so_far == num_images:
                    model.train(mode=was_training)
                    return
        model.train(mode=was_training)


# Training the model
def train_model(model, criterion, optimizer, scheduler, num_epochs=25):
    since = time.time()                                # 开始时间
    best_model_wts = copy.deepcopy(model.state_dict()) # 最佳参数
    best_acc = 0.0                                     # 最佳准确率

    for epoch in range(num_epochs):
        print('Epoch {}/{}'.format(epoch, num_epochs - 1))
        print('-' * 10)

        for phase in ['train', 'val']:                 # 训练或验证
            if phase == 'train':
                scheduler.step()
                model.train()  
            else:
                model.eval()   

            running_loss = 0.0
            running_corrects = 0

            for inputs, labels in dataloaders[phase]:          # 遍历数据集
                inputs = inputs.to(device)                     # [4, 3, 224, 224]
                labels = labels.to(device)                     # [4] , 4个标签值

                optimizer.zero_grad()                          # zero the parameter gradients
                with torch.set_grad_enabled(phase == 'train'): # 设置是否计算梯度
                    outputs = model(inputs)                    # [4, 2]
                    _, preds = torch.max(outputs, 1)           # [4] [4] 前者最大值，后者标签
                    loss = criterion(outputs, labels)          # [1]的结果，[4, 2]的预测值，[4]的标签
                    if phase == 'train':
                        loss.backward()   # 计算梯度
                        optimizer.step()  # 梯度更新

                running_loss += loss.item() * inputs.size(0)        # loss * batchsize
                running_corrects += torch.sum(preds == labels.data) # 正确预测数

            epoch_loss = running_loss / dataset_sizes[phase]             # 平均loss
            epoch_acc = running_corrects.double() / dataset_sizes[phase] # 预测正确率
            print('{} Loss: {:.4f} Acc: {:.4f}'.format(phase, epoch_loss, epoch_acc))

            if phase == 'val' and epoch_acc > best_acc:   # 将验证时最好的模型保存
                best_acc = epoch_acc
                best_model_wts = copy.deepcopy(model.state_dict())

    time_elapsed = time.time() - since
    print('Training complete in {:.0f}m {:.0f}s'.format(time_elapsed // 60, time_elapsed % 60))
    print('Best val Acc: {:4f}'.format(best_acc))
    
    model.load_state_dict(best_model_wts)  # load best model weights
    return model


#1 Finetuning the convnet
def finetune():
    model_ft = models.resnet18(pretrained=False)
    model_ft.load_state_dict(torch.load("resnet18-5c106cde.pth"))

    num_ftrs = model_ft.fc.in_features    # 上层神经元数
    model_ft.fc = nn.Linear(num_ftrs, 2)  # 调成二分类

    model_ft = model_ft.to(device)        

    criterion = nn.CrossEntropyLoss()
    optimizer_ft = optim.SGD(model_ft.parameters(), lr=0.001, momentum=0.9)
    exp_lr_scheduler = lr_scheduler.StepLR(optimizer_ft, step_size=7, gamma=0.1) # Decay LR by a factor of 0.1 every 7 epochs

    model_ft = train_model(model_ft, criterion, optimizer_ft, exp_lr_scheduler, num_epochs=25)
    visualize_model(model_ft)


#2 Convnet as fixed feature extractor
def extractor():
    model_conv = models.resnet18(pretrained=True)
    for param in model_conv.parameters():
        param.requires_grad = False

    num_ftrs = model_conv.fc.in_features
    model_conv.fc = nn.Linear(num_ftrs, 2) # requires_grad=True by default

    model_conv = model_conv.to(device)

    criterion = nn.CrossEntropyLoss()
    optimizer_conv = optim.SGD(model_conv.fc.parameters(), lr=0.001, momentum=0.9) # parameters of final layer are being optimized 
    exp_lr_scheduler = lr_scheduler.StepLR(optimizer_conv, step_size=7, gamma=0.1)

    model_conv = train_model(model_conv, criterion, optimizer_conv, exp_lr_scheduler, num_epochs=25)
    visualize_model(model_conv)


if __name__ == '__main__':
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    # 训练集数据增广和归一化，验证集数据仅归一化
    data_transforms = {
        'train': transforms.Compose([
            transforms.RandomResizedCrop(224),
            transforms.RandomHorizontalFlip(),
            transforms.ToTensor(),
            transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
        ]),
        'val': transforms.Compose([
            transforms.Resize(256),
            transforms.CenterCrop(224),
            transforms.ToTensor(),
            transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
        ]),
    }

    data_dir = 'hymenoptera_data'
    #/hymenoptera_data/train
    #                    ants/xx.jpg
    #                    bees/xx.jpg
    #/hymenoptera_data/val             
    #                    ants/xx.jpg
    #                    bees/xx.jpg

    image_datasets = {x: datasets.ImageFolder(os.path.join(data_dir, x), data_transforms[x])
                      for x in ['train', 'val']}
    #image_datasets['train']是一个整合所有类别文件夹下图片的集合                  
    #image_datasets['train'][0][0].shape     [3, 224, 224]  //第0条数据的图片    
    #image_datasets['train'][0][1]           0              //第0条数据的int标签
    #image_datasets['train'].classes         ['ants', 'bees']     
    #image_datasets['train'].class_to_idx    {'ants': 0, 'bees': 1}
    #image_datasets['train'].imgs            [('...jpg', 0), ('...jpg', 1), ... ()]
    #len(image_datasets['train'])            244

    dataset_sizes = {x: len(image_datasets[x]) for x in ['train', 'val']}
    class_names = image_datasets['train'].classes

    dataloaders = {x: torch.utils.data.DataLoader(image_datasets[x], batch_size=4, shuffle=True, num_workers=4)
                   for x in ['train', 'val']}            
    
    # visualize source picture
    inputs, classes = next(iter(dataloaders['train']))   # Get a batch of training data
    out = torchvision.utils.make_grid(inputs)            # Make a grid from batch
    imshow(out, title=[class_names[x] for x in classes])

    finetune()
    #2 extractor()

    #plt.show()
