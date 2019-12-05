#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = 'Bo Lin'

import math
import random
import numpy as np
from PIL import Image, ImageDraw, ImageFont


class Cluster(object):

    def __init__(self):
        self.pixels = []
        self.centroid = None

    def add_point(self, pixel):
        self.pixels.append(pixel)

    def set_new_centroid(self):
        R = [colour[0] for colour in self.pixels]
        G = [colour[1] for colour in self.pixels]
        B = [colour[2] for colour in self.pixels]

        R = sum(R) / len(R)
        G = sum(G) / len(G)
        B = sum(B) / len(B)

        self.centroid = (int(R), int(G), int(B))
        self.pixels = []

        return self.centroid


class Kmeans(object):

    def __init__(self, k=5, max_iterations=20, min_distance=5):
        self.k = k
        self.max_iterations = max_iterations
        self.min_distance = min_distance
        self.old_clusters = None
        self.image = None
        self.pixels = None
        self.clusters = None

    def run(self, image):
        self.image = image.convert("RGB")
        raw_pixels = np.array(image.getdata(), dtype=np.uint8)
        # 获取不重复像素
        unique_pixels = np.unique(raw_pixels, axis=0)
        # 从不重复像素里筛选出与背景色距离比较远的
        back_pixel = self.image.getpixel((0, 0))[:3]

        no_bg_pixels = []
        for pixel in unique_pixels:
            # if distance(pixel, back_pixel) > 10000: # hsv 距离阈值
            if self.calc_distance(pixel, back_pixel) > 60:
                no_bg_pixels.append(pixel)

        self.pixels = no_bg_pixels
        # print("不重复像素", self.pixels)

        self.clusters = [None for i in range(self.k)]

        random_pixels = random.sample(self.pixels, self.k)
        # print(randomPixels)
        for idx in range(self.k):
            self.clusters[idx] = Cluster()
            self.clusters[idx].centroid = random_pixels[idx]
        iterations = 0
        while self.should_exit(iterations) is False:
            self.old_clusters = [cluster.centroid for cluster in self.clusters]
            for pixel in self.pixels:
                self.assign_clusters(pixel)
            for cluster in self.clusters:
                cluster.set_new_centroid()
            iterations += 1
        return [cluster.centroid for cluster in self.clusters]

    def assign_clusters(self, pixel):
        nearest = None
        shortest = float('Inf')
        for cluster in self.clusters:
            distance_c = self.calc_distance(cluster.centroid, pixel)
            if distance_c < shortest:
                shortest = distance_c
                nearest = cluster

        nearest.add_point(pixel)

    @staticmethod
    def calc_distance(a, b):
        result = np.sqrt(sum((a - b) ** 2))
        return result

    def should_exit(self, iterations):

        if self.old_clusters is None:
            return False

        for idx in range(self.k):
            dist = self.calc_distance(
                np.array(self.clusters[idx].centroid),
                np.array(self.old_clusters[idx])
            )
            if dist < self.min_distance:
                return True

        if iterations <= self.max_iterations:
            return False

        return True


def analyze_pic(image):
    from image_functions.fit_in import get_main_body
    image_rgb = image.convert("RGB")
    image = get_main_body(image_rgb)
    k = Kmeans()
    while True:
        try:
            result = k.run(image)
            break
        except Exception as e:
            pass
    print("生成结果", result)
    pic = Image.new("RGB", (k.k * 200, 100), result[0])

    set_font = ImageFont.truetype('fonts/calibri.ttf', 20)
    fill_color = "#FFFFFF"

    for i in range(1, k.k):

        pic.paste(Image.new("RGB", (200, 100), result[i]), (i * 200, 0))

    draw = ImageDraw.Draw(pic)
    for i in range(k.k):
        hex_color = rgb_to_hex(result[i])
        draw.text((i * 200 + 100, 50), hex_color, font=set_font, fill=fill_color)
    return pic


def rgb2hsv(color):
    r, g, b = color[0] / 255.0, color[1] / 255.0, color[2] / 255.0
    mx = max(r, g, b)
    mn = min(r, g, b)
    df = mx - mn
    h = 0
    if mx == r and g >= b:
        h = 60 * ((g - b) / df) + 0
    elif mx == r and g < b:
        h = 60 * ((g - b) / df) + 360
    elif mx == g:
        h = 60 * ((b - r) / df) + 120
    elif mx == b:
        h = 60 * ((r - g) / df) + 240

    if mx == 0:
        s = 0
    else:
        s = df / mx
    v = mx
    return h, s, v


def rgb_to_hex(rgb):
    color = '#'
    for i in rgb:
        num = int(i)
        # 将R、G、B分别转化为16进制拼接转换并大写  hex() 函数用于将10进制整数转换成16进制，以字符串形式表示
        color += str(hex(num))[-2:].replace('x', '0').upper()
    # print(color)
    return color


def distance(pixel, back_pixel):
    h1, s1, v1 = rgb2hsv(pixel)
    h2, s2, v2 = rgb2hsv(back_pixel)
    dis = math.pow(math.fabs(h1 - h2), 2) + math.pow(math.fabs(s1 - s2), 2) + math.pow(math.fabs(v1 - v2), 2)
    # print(pixel, back_pixel,"两个距离",dis)
    return dis


if __name__ == "__main__":
    image_i = Image.open("../examples/images/10.jpg")
    analyze_pic(image_i)
    # rgb_to_hex((227, 181, 102))

