import torch
import torchvision
import torchvision.transforms as T
import cv2
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import random

def get_prediction(img_path, threshold):
    img = Image.open(img_path)
    transform = T.Compose([T.ToTensor()])
    img = transform(img)
    pred = model([img])
    #print(pred)

    pred_score = list(pred[0]['scores'].detach().numpy())
    pred_t = [pred_score.index(x) for x in pred_score if x>threshold][-1] # 1 代表最后一个符合的索引

    pred_boxes = [[(i[0], i[1]), (i[2], i[3])] for i in list(pred[0]['boxes'].detach().numpy())]
    pred_boxes = pred_boxes[:pred_t+1]
    #print(pred_boxes)

    pred_keypoints = list(pred[0]['cv-keypoints'].detach().numpy())[:pred_t+1]

    return pred_boxes, pred_keypoints

def random_colour_masks(image):
    colours = [[0, 255, 0],[0, 0, 255],[255, 0, 0],[0, 255, 255],[255, 255, 0],[255, 0, 255],[80, 70, 180],[250, 80, 190],[245, 145, 50],[70, 150, 250],[50, 190, 190]]
    r = np.zeros_like(image).astype(np.uint8)
    g = np.zeros_like(image).astype(np.uint8)
    b = np.zeros_like(image).astype(np.uint8)
    r[image == 1], g[image == 1], b[image == 1] = colours[random.randrange(0,10)]
    coloured_mask = np.stack([r, g, b], axis=2)
    return coloured_mask

def keypointrcnn_api(img_path, threshold=0.9, rect_th=3, text_size=3, text_th=3):
    boxes, keypoints = get_prediction(img_path, threshold)
    img = cv2.imread(img_path)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    
    for i in range(len(boxes)):
        cv2.rectangle(img, boxes[i][0], boxes[i][1],color=(0, 255, 0), thickness=rect_th)
        for j in range(17):
            cv2.circle(img, (keypoints[i][j][0],keypoints[i][j][1]),10,(255,0,0),-1)
    
    plt.figure(figsize=(20,30))
    plt.imshow(img)
    plt.xticks([])
    plt.yticks([])
    plt.show()


model = torchvision.models.detection.keypointrcnn_resnet50_fpn(pretrained=True)
model.eval()

img_path = './2.jpg'
keypointrcnn_api(img_path)




#instance_segmentation_api('./photo.jpg')