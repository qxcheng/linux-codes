#彩色直方图
# 本质：统计每个像素灰度 出现的概率 0-255 p
import cv2
import numpy as np
import matplotlib.pyplot as plt
img = cv2.imread('image0.jpg',1)
imgInfo = img.shape
height = imgInfo[0]
width = imgInfo[1]

count_b = np.zeros(256,np.float)
count_g = np.zeros(256,np.float)
count_r = np.zeros(256,np.float)
for i in range(0,height):
    for j in range(0,width):
        (b,g,r) = img[i,j]
        index_b = int(b)
        index_g = int(g)
        index_r = int(r)
        count_b[index_b] = count_b[index_b]+1
        count_g[index_g] = count_g[index_g]+1
        count_r[index_r] = count_r[index_r]+1
for i in range(0,256):
    count_b[i] = count_b[i]/(height*width)
    count_g[i] = count_g[i]/(height*width)
    count_r[i] = count_r[i]/(height*width)
x = np.linspace(0,255,256)
y1 = count_b
plt.figure()
plt.bar(x,y1,0.9,alpha=1,color='b')
y2 = count_g
plt.figure()
plt.bar(x,y2,0.9,alpha=1,color='g')
y3 = count_r
plt.figure()
plt.bar(x,y3,0.9,alpha=1,color='r')
plt.show()
cv2.waitKey(0)