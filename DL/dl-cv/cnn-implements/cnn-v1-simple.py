import torchvision
import numpy as np
import cupy as cp
import os
import time, functools

## 标签one-hot处理
def onehot(targets, num):
    result = cp.zeros((num, 10))
    for i in range(num):
        result[i][targets[i]] = 1
    return result

## ReLU
def relu(x):
    return cp.maximum(0.01*x, x)

## ReLU导数
def Drelu(x):
    x = cp.where(x<=0, 0.01, 1)
    return x

## 输入特征图填充处理
def padding(x, pad):
    c, w, h = x.shape
    result = cp.zeros((c, w+2*pad, h+2*pad))
    result[:, pad:w+pad, pad:h+pad] = x
    return result

## 卷积层前向传播
def conv_forward(x, kernel, b, stride=1, pad=0):
    if pad != 0:
        x = padding(x, pad)
    cx, wx, hx = x.shape
    nk, ck, wk, hk = kernel.shape        # kernel的个数、通道数、宽、高
    feature_w = (wx - wk) // stride + 1  # 返回的特征图尺寸
    feature = cp.zeros((nk, feature_w, feature_w))

    for n in range(nk):
        for i in range(feature_w):
            for j in range(feature_w):
                feature[n, i, j] = cp.sum(kernel[n] * x[:, i*stride:i*stride+wk, j*stride:j*stride+hk]) + b[n]

    return feature

## 池化层前向传播
def pool_forward(x):
    c, w, h = x.shape
    feature_w = w // 2
    feature = cp.zeros((c, feature_w, feature_w))
    feature_mask = cp.zeros((c, w, h))   # 记录最大池化时最大值的位置信息用于反向传播

    for ci in range(c):
        for i in range(feature_w):
            for j in range(feature_w):
                feature[ci, i, j] = cp.max(x[ci,i*2:i*2+2,j*2:j*2+2])
                index = cp.argmax(x[ci,i*2:i*2+2,j*2:j*2+2])
                if index == 0:    # 左上角为最大值
                    feature_mask[ci, i * 2, j * 2] = 1
                elif index == 1:  # 右上角为最大值
                    feature_mask[ci, i * 2, j * 2 + 1] = 1
                elif index == 2:  # 左下角为最大值
                    feature_mask[ci, i * 2 + 1, j * 2] = 1
                else:             # 右下角为最大值
                    feature_mask[ci, i * 2 + 1, j * 2 + 1] = 1

    return feature, feature_mask

## 池化层反向传播
def pool_backward(d2, mask):
    c,w,h = mask.shape
    d1 = cp.zeros((c, w, h))  # bx16x10x10
    for ci in range(c):
        for i in range(w):
            for j in range(h):
                d1[ci, i, j] = d2[ci, i // 2, j // 2]
    d1 = d1 * mask
    return d1

## 卷积层反向传播
def conv_backward(a1, w2, d2, stride=1):
    ca, ha, wa = a1.shape      # bx6x14x14
    nw, cw, hw, ww = w2.shape  # 16x6x5x5
    cd, hd, wd = d2.shape      # bx16x10x10

    dw2 = cp.zeros((nw,cw,hw,ww))
    db2 = cp.zeros((nw))
    d1 = cp.zeros((ca, ha, wa))

    # 计算dw2
    pad_a1 = a1
    if ha-hd+1 != hw:
        pad = (hw-1+hd-ha) // 2
        pad_a1 = padding(a1, pad)
    for n in range(nw):
        for c in range(cw):
            for i in range(hw):
                for j in range(ww):
                    dw2[n, c, i, j] += cp.sum(d2[n] * pad_a1[c, i*stride:i*stride+hd, j*stride:j*stride+wd])

    # 计算db2
    for n in range(cd):
        for i in range(hd):
            for j in range(wd):
                db2[n] += cp.sum(d2[n, i, j])

    # 计算d1
    w2_180 = cp.flip(w2, 3)       # 矩阵旋转180度
    w2_180 = cp.flip(w2_180, -2)
    w2_180 = w2_180.reshape((cw, nw, hw, ww))
    pad_d2 = d2
    if hd-hw+1 != ha:
        pad = (ha-hd+hw-1) // 2
        pad_d2 = padding(d2, pad)
    for c in range(ca):
        for i in range(ha):
            for j in range(wa):
                d1[c, i, j] = cp.sum(w2_180[c] * pad_d2[:, i*stride:i*stride+hw, j*stride:j*stride+ww])

    return dw2, db2, d1


class CNN():
    def __init__(self, lr=0.01):
        self.lr = lr
        self.beta1 = 0.95
        self.beta2 = 0.99

        self.k1 = cp.random.randn(6,1,5,5) * 0.01
        self.b1 = cp.zeros(6)
        self.w3 = cp.random.randn(864,400) / cp.sqrt(864)
        self.b3 = cp.zeros(400)
        self.w4 = cp.random.randn(400,10) / cp.sqrt(400)
        self.b4 = cp.zeros(10)

        #momentum
        self.vk1 = cp.zeros(self.k1.shape)
        self.vb1 = cp.zeros(self.b1.shape)
        self.vw3 = cp.zeros(self.w3.shape)
        self.vb3 = cp.zeros(self.b3.shape)
        self.vw4 = cp.zeros(self.w4.shape)
        self.vb4 = cp.zeros(self.b4.shape)

        #RMS params
        self.sk1 = cp.zeros(self.k1.shape)
        self.sb1 = cp.zeros(self.b1.shape)
        self.sw3 = cp.zeros(self.w3.shape)
        self.sb3 = cp.zeros(self.b3.shape)
        self.sw4 = cp.zeros(self.w4.shape)
        self.sb4 = cp.zeros(self.b4.shape)

    def forward(self, x, y):
        self.x  = cp.asarray(x)
        #print(self.x[0])
        self.z1 = conv_forward(self.x, self.k1, self.b1, stride=1, pad=0)   # bx6x24x24
        #print(self.z1)
        self.a1 = relu(self.z1)
        self.p2, self.mask2 = pool_forward(self.a1)                         # bx6x12x12 bx6x24x24
        self.f0 = self.p2.reshape(1, 12*12*6)                      # 池化层展开到全连接层
        self.z3 = cp.dot(self.f0, self.w3) + self.b3                        # bx128
        self.a3 = relu(self.z3)
        self.z4 = cp.dot(self.a3, self.w4) + self.b4                        # bx10

        #M = cp.tile(cp.max(self.z4, axis=1).reshape(self.batch, 1), 10)   # 防止溢出
        self.a4 = cp.exp(self.z4) / cp.tile(cp.sum(cp.exp(self.z4), axis=1), 10)  # Softmax层
        #print(self.a4)
        loss = - cp.sum(y * cp.log(self.a4))        # 交叉熵
        self.d4 = self.a4 - y                                   # bx10 , 用于反向传播

        return loss, self.a4

    def backward(self):
        dw4 = cp.dot(self.a3.T, self.d4)        # 128 x 10
        db4= cp.sum(self.d4, axis=0)            # 10
        d3  = cp.dot(self.d4, self.w4.T) * Drelu(self.z3)   # b x 128
        dw3 = cp.dot(self.f0.T, d3)             # 864 x 128
        db3 = cp.sum(d3, axis=0)               # 128
        dp2 = cp.dot(d3, self.w3.T)                         # b x 864

        dp2 = dp2.reshape(6, 12, 12)                        # bx6x12x12
        dp2 = pool_backward(dp2, self.mask2) * Drelu(self.z1)           # bx6x12x12
        dk1, db1, _ = conv_backward(self.x, self.k1, dp2, stride=1)

        # print(self.w4[0])
        # print(dw4[0])
        #Adam
        self.vk1 = self.beta1 * self.vk1 + (1 - self.beta1) * dk1
        self.sk1 = self.beta2 * self.sk1 + (1 - self.beta2) * (dk1**2)
        self.k1 -= self.lr * self.vk1 / cp.sqrt(self.sk1 + 1e-7)

        self.vb1 = self.beta1 * self.vb1 + (1 - self.beta1) * db1
        self.sb1 = self.beta2 * self.sb1 + (1 - self.beta2) * (db1 ** 2)
        self.b1 -= self.lr * self.vb1 / cp.sqrt(self.sb1 + 1e-7)

        self.vw3 = self.beta1 * self.vw3 + (1 - self.beta1) * dw3
        self.sw3 = self.beta2 * self.sw3 + (1 - self.beta2) * (dw3 ** 2)
        self.w3 -= self.lr * self.vw3 / cp.sqrt(self.sw3 + 1e-7)

        self.vb3 = self.beta1 * self.vb3 + (1 - self.beta1) * db3
        self.sb3 = self.beta2 * self.sb3 + (1 - self.beta2) * (db3 ** 2)
        self.b3 -= self.lr * self.vb3 / cp.sqrt(self.sb3 + 1e-7)

        self.vw4 = self.beta1 * self.vw4 + (1 - self.beta1) * dw4
        self.sw4 = self.beta2 * self.sw4 + (1 - self.beta2) * (dw4 ** 2)
        self.w4 -= self.lr * self.vw4 / cp.sqrt(self.sw4 + 1e-7)

        self.vb4 = self.beta1 * self.vb4 + (1 - self.beta1) * db4
        self.sb4 = self.beta2 * self.sb4 + (1 - self.beta2) * (db4 ** 2)
        self.b4 -= self.lr * self.vb4 / cp.sqrt(self.sb4 + 1e-7)


def train():
    cnn = CNN(lr=0.01)
    for epoch in range(10):
        for i in range(0, 60000):
            X = train_data.data[i]
            y = train_data.targets[i]
            loss, _ = cnn.forward(X, y)
            cnn.backward()
            print("Epoch-{}-{:05d}".format(str(epoch), i), ":", "loss:{:.3f}".format(cp.asnumpy(loss)), "lr:{:.5f}".format(cnn.lr))

        cnn.lr *= 0.95**(epoch+1)
        cp.savez("data2.npz", k1=cnn.k1, b1=cnn.b1, k2=cnn.k2, b2=cnn.b2, w3=cnn.w3, b3=cnn.b3, w4=cnn.w4, b4=cnn.b4)

def eval():
    r = cp.load("data2.npz")
    cnn = CNN(batch=10000)
    cnn.k1 = r["k1"]
    cnn.b1 = r["b1"]
    cnn.k2 = r["k2"]
    cnn.b2 = r["b2"]
    cnn.w3 = r["w3"]
    cnn.b3 = r["b3"]
    cnn.w4 = r["w4"]
    cnn.b4 = r["b4"]

    acc_num = 0
    _, result = cnn.forward(test_data.data, test_data.targets2)
    result = cp.asnumpy(cp.argmax(result, axis=1))
    acc_num += np.sum(result == test_data.targets)
    precison = acc_num / 10000
    print("Precison:", precison)

def conv_test():
    X = cp.random.randn(12, 1, 6, 6)
    K = cp.random.randn(3, 1, 3, 3)
    b = cp.zeros(3)

    Y_true = cp.ones((12, 3, 2, 2))

    for i in range(10000):
        for j in range(12):
            # 前向
            x = X[j]
            y_true = Y_true[j]

            z = conv_forward(x, K, b, stride=1, pad=0)
            a = relu(z)
            pool_z, mask = pool_forward(a)
            # 反向
            loss = cp.sum((pool_z - y_true) * (pool_z - y_true)) / 2
            dy = pool_z - y_true
            pool_d = pool_backward(dy, mask) * Drelu(z)
            dK, db, _ = conv_backward(x, K, pool_d, stride=1)
            # 更新梯度
            K -= 0.01 * dK
            b -= 0.01 * db

            # 打印损失
            print("step:{} loss:{:.3f}".format(i, cp.asnumpy(loss)))




if __name__ == '__main__':

    # Mnist手写数字集
    dataset_path = "D://datasets//mnist"
    train_data = torchvision.datasets.MNIST(root=dataset_path, train=True, download=False)
    test_data = torchvision.datasets.MNIST(root=dataset_path, train=False)
    train_data.data = train_data.data.numpy()         # [60000,28,28]
    train_data.targets = train_data.targets.numpy()   # [60000]
    test_data.data = test_data.data.numpy()           # [10000,28,28]
    test_data.targets = test_data.targets.numpy()     # [10000]

    # 输入向量处理
    train_data.data = train_data.data.reshape(60000, 1, 28, 28) / 255.
    np.random.shuffle(train_data.data)
    test_data.data = test_data.data.reshape(10000, 1, 28, 28) / 255.

    # 标签one-hot处理
    train_data.targets = onehot(train_data.targets, 60000) # (60000, 10)
    test_data.targets2 = onehot(test_data.targets, 10000)  # 测试时用于前向传播

    #conv_test()
    train()
    #eval()



