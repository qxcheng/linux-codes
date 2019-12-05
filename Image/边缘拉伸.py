#-*- coding : utf-8-*-
from PIL import Image
import numpy as np
import random

def border_fill(img_rgba, height, width, limit, ratio_up=0.5, ratio_left=0.5):
    '''params
    img: 原图
    height: 拉伸后的高度
    width: 拉伸后的宽度
    limit: 指定的取填充像素的宽度范围
    ratio_left ：左边拉伸占左右拉伸总比
    ratio_up: 上边拉伸占上下拉伸总比
    return: string or Image object
    '''
    img_rgba = np.array(img_rgba)

    h, w, c = img_rgba.shape
    if h == height and w == width:  # 不拉伸
        return True, Image.fromarray(img_rgba)

    pad_left = int((width - w) * ratio_left)
    pad_right = width - w - pad_left
    pad_up = int((height - h) * ratio_up)
    pad_down = height - h - pad_up
    dst = np.zeros((height, width, c), np.uint8)
    dst[pad_up:pad_up + h, pad_left:pad_left + w] = img_rgba

    if h == height and w != width:  # 只拉伸宽度
        #print("只拉伸宽度")
        for col in range(pad_left):
            pixel = random.randint(pad_left, pad_left + limit)
            dst[:,col] = dst[:,pixel]
        for col in range(width-pad_right, width):
            pixel = random.randint(width-pad_right-limit, width-pad_right-1)
            dst[:,col] = dst[:,pixel]
    elif h != height and w == width:
        #print("只拉伸高度")
        for row in range(pad_up):
            pixel = random.randint(pad_up, pad_up+limit)
            dst[row,:] = dst[pixel,:]
        for row in range(height-pad_down, height):
            pixel = random.randint(height-pad_down-limit, height-pad_down-1)
            dst[row,:] = dst[pixel,:]
    else:
        #print("拉伸宽度和高度")
        for col in range(pad_left):
            pixel = random.randint(pad_left, pad_left + limit)
            dst[:,col] = dst[:,pixel]
        for col in range(width-pad_right, width):
            pixel = random.randint(width-pad_right-limit, width-pad_right-1)
            dst[:,col] = dst[:,pixel]
        for row in range(pad_up):
            pixel = random.randint(pad_up, pad_up+limit)
            dst[row,:] = dst[pixel,:]
        for row in range(height-pad_down, height):
            pixel = random.randint(height-pad_down-limit, height-pad_down-1)
            dst[row,:] = dst[pixel,:]

    return True, Image.fromarray(dst)