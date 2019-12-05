#中值滤波
# 中值滤波 3*3 
import cv2
import numpy as np
img = cv2.imread('image11.jpg',1)
imgInfo = img.shape
height = imgInfo[0]
width = imgInfo[1]
img = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
cv2.imshow('src',img)
dst = np.zeros((height,width,3),np.uint8)
collect = np.zeros(9,np.uint8)
for i in range(1,height-1):
    for j in range(1,width-1):
        k = 0
        for m in range(-1,2):
            for n in range(-1,2):
                gray = img[i+m,j+n]
                collect[k] = gray
                k = k+1
        # 0 1 2 3 4 5 6 7 8
        #   1 
        for k in range(0,9):
            p1 = collect[k]
            for t in range(k+1,9):
                if p1<collect[t]:
                    mid = collect[t]
                    collect[t] = p1
                    p1 = mid
        dst[i,j] = collect[4]
cv2.imshow('dst',dst)
cv2.waitKey(0)