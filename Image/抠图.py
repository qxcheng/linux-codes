#-*- coding : utf-8-*-
import cv2
from PIL import Image
import numpy as np


# 抠图
def cutOut(img, mode_index, crop_mode):

    img = cv2.cvtColor(np.array(img), cv2.COLOR_RGBA2RGB)  # 要从rgba转换回rgb

    filter_lib = [[35, 25, 85], [35, 25, 80], [55, 30, 125], [35, 20, 85], [35, 45, 100]]

    mode = filter_lib[mode_index]
    filter_size, canny_low, canny_high = mode[0], mode[1], mode[2]
    blur = cv2.bilateralFilter(img, 9, filter_size, filter_size)
    edge = cv2.Canny(blur, canny_low, canny_high)
    x, y, w, h = bbox(edge, 255)  # this is also used for crop
    mask = np.zeros(img.shape[:2], np.uint8)
    bgdModel = np.zeros((1, 65), np.float64)
    fgdModel = np.zeros((1, 65), np.float64)
    rect = (x, y, w, h)
    cv2.grabCut(img, mask, rect, bgdModel, fgdModel, 5, cv2.GC_INIT_WITH_RECT)
    mask2 = np.where((mask == 2) | (mask == 0), 0, 1).astype('uint8')
    median = cv2.medianBlur(mask2, 7)
    img_1 = img * median[:, :, np.newaxis]

    # 将抠好的图片转化为png格式
    b_channel, g_channel, r_channel = cv2.split(img_1)
    # 将图像的背景部分设为透明
    a_channel = np.where((median == 2) | (median == 0), 0, 255).astype('uint8')
    # a_channel = np.where((median == 0), 0, 255).astype('uint8')
    img_final = cv2.merge((b_channel, g_channel, r_channel, a_channel))
    # 得到最终抠好的png图片
    if crop_mode:  # 如果crop_mode打开，则只扣除中间的图
        img_crop = Image.fromarray(img_final)
        # img_final = img_crop.crop((x,y,x+w,y+h)) #改成 getbbox

        _,_,_,a = img_crop.split()

        img_final = img_crop.crop(a.getbbox())

    return np.array(img_final)

# 抠图依赖1
def check_cor(img, i_iter, j_iter, thres, con1, con2, value):
    for i in range(i_iter):
        count = 0
        if con1:
            cur = i
        else:
            cur = i_iter - i - 1

        for j in range(j_iter):
            if con2:
                a = cur
                b = j
            else:
                a = j
                b = cur
            if value == 255:
                if img[a, b] == value:  # 如果是白色的边缘
                    count += 1
                    if count > thres:
                        return cur
            else:
                if img[a, b] <= value:  # 如果是白色的边缘
                    count += 1
                    if count > thres:
                        return cur

# 抠图依赖2
def bbox(img, value):
    cols, rows = img.shape
    Threshold = 1  # 默认设置阀值为0,即发现1次白点即算边缘
    y0 = check_cor(img, cols, rows, Threshold, True, True, value)
    x0 = check_cor(img, rows, cols, Threshold, True, False, value)
    y1 = check_cor(img, cols, rows, Threshold, False, True, value)
    x1 = check_cor(img, rows, cols, Threshold, False, False, value)
    return x0, y0, x1 - x0, y1 - y0

# bbox改进版
def bbox_b(img, value):
    rows, cols = img.shape

    for i in range(rows):
        if any(img[i]):
            if sum(img[i] <= value) > 1:
                y_min = i
                break
    for i in range(rows):
        if any(img[rows-1-i]):
            if sum(img[rows-1-i] <= value) > 1:
                y_max = rows-1-i
                break
    for i in range(cols):
        if any(img[:,i]):
            if sum(img[:,i] <= value) > 1:
                x_min = i
                break
    for i in range(cols):
        if any(img[:,cols-1-i]):
            if sum(img[:,cols-1-i] <= value) > 1:
                x_max = cols-1-i
                break
    return x_min, y_min, x_max-x_min, y_max-y_min

if __name__ == '__main__':
    img = Image.open('1.jpg')
    img = cutOut(img, 0, False)
    img = Image.fromarray(img)
    img.show()
