#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = 'Bo Lin'

import cv2
import numpy as np
from PIL import Image


# 题目4 要求的功能
def fit_in(image, min_margin=50, bg_size=(800, 800)):
    """
    :param image: Image object
    :param min_margin: 样例蓝色边距
    :param bg_size: 背景画布大小
    :return: Image object
    """
    # 先获取体
    image = get_main_body(image)
    print(image, 'image')
    # 计算红框区域
    inner_size = (bg_size[0] - min_margin, bg_size[1] - min_margin)
    # 计算缩放比
    width_ratio = image.size[0] / inner_size[0]
    height_ratio = image.size[1] / inner_size[1]
    ratio = max(width_ratio, height_ratio)
    # 对图片进行缩放
    image_resized = image.resize((int(image.size[0] / ratio), int(image.size[1] / ratio)), Image.ANTIALIAS)
    # 创建背景画布
    color = image_resized.getpixel((0, 0))
    bg_image = Image.new(image.mode, bg_size, color)
    # 计算各边边距，居中放置主体（此比赛版本只支持居中）
    x_margin = (bg_image.size[0] - image_resized.size[0]) >> 1
    y_margin = (bg_image.size[1] - image_resized.size[1]) >> 1
    bg_image.paste(image_resized, (x_margin, y_margin))
    return bg_image


# 获取图片主体
def get_main_body(image):
    if image.mode == "RGBA":
        r, g, b, a = image.split()
        (min_alpha, max_alpha) = a.getextrema()
        if min_alpha < max_alpha:  # 有背景
            crop_region = a.getbbox()
        else:
            img2 = image.convert("L")  # 灰度图
            img2 = np.array(img2)
            img2 = gamma_trans(img2, gamma=9)
            back_pix = img2[10][10]
            x0, y0, w, h = bbox_b(img2, max(0, int(back_pix * 2 / 3)))
            crop_region = (x0, y0, x0 + w, y0 + h)
    else:
        img2 = image.convert("L")  # 灰度图
        img2 = np.array(img2)
        img2 = gamma_trans(img2, gamma=9)
        back_pix = img2[10][10]
        x0, y0, w, h = bbox_b(img2, max(0, int(back_pix * 2 / 3)))
        crop_region = (x0, y0, x0 + w, y0 + h)
    return image.crop(crop_region)


# 让图片锐化
def gamma_trans(img, gamma):
    # 具体做法先归一化到1，然后gamma作为指数值求出新的像素值再还原
    gamma_table = [np.power(x / 255.0, gamma) * 255.0 for x in range(256)]
    gamma_table = np.round(np.array(gamma_table)).astype(np.uint8)
    # 实现映射用的是Opencv的查表函数
    return cv2.LUT(img, gamma_table)


# 找到主题边缘
def bbox_b(img, value):
    rows, cols = img.shape
    y_max, y_min = get_y(img, rows, value)
    x_max, x_min = get_x(img, cols, value)
    return x_min, y_min, x_max - x_min, y_max - y_min


def get_x(img, cols, value):
    """
    获取X
    :param img:
    :param cols:
    :param value:
    :return:
    """
    x_max = None
    x_min = None
    for i in range(cols):
        if any(img[:, i]) and sum(np.where(img[:, i] <= value, img[:, i], 0)):
            x_min = i
            break
    for i in range(cols):
        if any(img[:, cols - 1 - i]) and sum(np.where(img[:, cols - 1 - i] <= value, img[:, cols - 1 - i], 0)):
            x_max = cols - 1 - i
            break
    return x_max, x_min


def get_y(img, rows, value):
    """
    获取Y
    :param img: 图像的矩阵信息
    :param rows:
    :param value:
    :return:
    """
    y_max = None
    y_min = None
    for i in range(rows):
        if any(img[i]) and sum(np.where(img[i] <= value, img[i], 0)):
            y_min = i
            break
    for i in range(rows):
        if any(img[rows - 1 - i]) and sum(np.where(img[rows - 1 - i] <= value, img[rows - 1 - i], 0)):
            y_max = rows - 1 - i
            break
    return y_max, y_min


if __name__ == "__main__":
    image_i = Image.open("../examples/images/1.jpg")
    image_final = fit_in(image_i)
    image_final.show()
