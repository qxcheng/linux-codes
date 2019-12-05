import torchvision
import numpy as np
import cupy as cp
import time, functools


## 标签one-hot处理
def onehot(targets, num):
    result = cp.zeros((num, 10))
    for i in range(num):
        result[i][targets[i]] = 1
    return result

# sigmoid
def sigmoid(x):
    return 1 / (1 + cp.exp(-x))

# sigmoid的一阶导数
def Dsigmoid(x):
    return sigmoid(x)*(1-sigmoid(x))

## ReLU激活函数
def relu(x):
    return cp.maximum(0.01*x, x)

## ReLU激活函数的一阶导数
def Drelu(x):
    x = cp.where(x<=0, 0.01, 1)
    return x


## 输入特征图填充处理
def padding(x, pad):
    b, c, w, h = x.shape
    result = cp.zeros((b, c, w+2*pad, h+2*pad))
    result[:, :, pad:w+pad, pad:h+pad] = x
    return result

## 卷积层前向传播
def conv_forward(x, kernel, b, stride=1, pad=0):
    if pad != 0:
        x = padding(x, pad)
    bx, cx, wx, hx = x.shape
    nk, ck, wk, hk = kernel.shape        # kernel的个数、通道数、宽、高
    feature_w = (wx - wk) // stride + 1  # 返回的特征图尺寸
    feature = cp.zeros((bx, nk, feature_w, feature_w))

    for n in range(nk):
        for i in range(feature_w):
            for j in range(feature_w):
                feature[:, n, i, j] = cp.sum(kernel[n] * x[:, :, i*stride:i*stride+wk, j*stride:j*stride+hk]) + b[n]

    return feature

## 池化层前向传播
def pool_forward(x):
    b, c, w, k = x.shape
    feature_w = w // 2
    feature = cp.zeros((b, c, feature_w, feature_w))
    feature_mask = cp.zeros((b, c, w, k))   # 记录最大池化时最大值的位置信息用于反向传播

    for ci in range(c):
        for i in range(feature_w):
            for j in range(feature_w):
                feature[:, ci, i, j] = cp.max(x[:,ci,i*2:i*2+2,j*2:j*2+2])
                index = cp.argmax(x[:,ci,i*2:i*2+2,j*2:j*2+2])
                if index == 0:                              # 左上角为最大值
                    feature_mask[:,ci, i*2, j*2] = 1
                elif index == 1:                            # 右上角为最大值
                    feature_mask[:,ci, i*2, j*2+1] = 1
                elif index == 2:                            # 左下角为最大值
                    feature_mask[:,ci, i*2+1, j*2] = 1
                else:                                       # 右下角为最大值
                    feature_mask[:,ci, i*2+1, j*2+1] = 1

    return feature, feature_mask

## 池化层反向传播
def pool_backward(d2, mask, z1):
    b,c,h,w = mask.shape
    d1 = cp.zeros((b,c, h, w))  # 16x10x10

    for ci in range(c):
        for i in range(h):
            for j in range(w):
                d1[:, ci, i, j] = d2[:, ci, i // 2, j // 2]
    d1 = d1 * mask * Drelu(z1)
    return d1

## 卷积层反向传播
def conv_backward(a1, w2, d2, stride=1):
    ba, ca, ha, wa = a1.shape      # bx6x14x14
    nw, cw, hw, ww = w2.shape      # 16x6x5x5
    bd, cd, hd, wd = d2.shape      # bx16x10x10

    dw2 = cp.zeros((nw,cw,hw,ww))
    db2 = cp.zeros((nw))
    d1 = cp.zeros((ba, ca, ha, wa))

    # 计算dw2
    pad_a1 = a1
    if ha-hd+1 != hw:
        pad = (hw-1+hd-ha) // 2
        pad_a1 = padding(a1, pad)
    for n in range(nw):
        for c in range(cw):
            for i in range(hw):
                for j in range(ww):
                    dw2[n, c, i, j] += cp.sum(d2[:, n] * pad_a1[:, c, i*stride:i*stride+hd, j*stride:j*stride+wd])
                    dw2[n, c, i, j] /= ba
    # 计算db2
    for n in range(cd):
        for i in range(hd):
            for j in range(wd):
                db2[n] += cp.sum(d2[:, n, i, j])
                db2[n] /= ba

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
                d1[:, c, i, j] = cp.sum(w2_180[c] * pad_d2[:, :, i*stride:i*stride+hw, j*stride:j*stride+ww])

    return dw2, db2, d1


class LeNet():
    def __init__(self, batch=3, lr=0.01):
        self.batch = batch
        self.lr = lr

        self.k1 = cp.random.standard_normal((6,1,5,5))  * 0.01
        self.b1 = cp.zeros(6)
        self.k2 = cp.random.standard_normal((16,6,5,5)) * 0.01
        self.b2 = cp.zeros(16)

        self.w3 = cp.random.normal(loc=0, scale=1.0/cp.sqrt(400*120), size=(400,120))
        self.b3 = cp.zeros(120)
        self.w4 = cp.random.normal(loc=0, scale=1.0/cp.sqrt(120*84), size=(120,84))
        self.b4 = cp.zeros(84)
        self.w5 = cp.random.normal(loc=0, scale=1.0/cp.sqrt(84*10), size=(84,10))
        self.b5 = cp.zeros(10)

    def forward(self, x, y):
        self.x  = cp.asarray(x)
        self.z1 = conv_forward(self.x, self.k1, self.b1, stride=1, pad=2)   # bx6x28x28
        self.a1 = relu(self.z1)
        self.p1, self.mask1 = pool_forward(self.a1)                         # bx6x14x14 bx6x28x28
        self.z2 = conv_forward(self.p1, self.k2, self.b2, stride=1, pad=0)  # bx16x10x10
        self.a2 = relu(self.z2)
        self.p2, self.mask2 = pool_forward(self.a2)                         # bx16x5x5 bx16x10x10
        self.f0 = self.p2.reshape(self.batch, 5*5*16)                       # 池化层展开到全连接层
        self.z3 = cp.dot(self.f0, self.w3) + self.b3                        # bx120
        self.a3 = relu(self.z3)
        self.z4 = cp.dot(self.a3, self.w4) + self.b4                        # bx84
        self.a4 = relu(self.z4)
        self.z5 = cp.dot(self.a4, self.w5) + self.b5                        # bx10
        #print(self.z5[0])
        self.a5 = cp.exp(self.z5 - cp.tile(cp.max(self.z5, axis=1).reshape(self.batch, 1), 10))    # 防止溢出
        self.a5 = self.a5 / cp.tile(cp.sum(self.a5, axis=1).reshape(self.batch, 1), 10)  # Softmax层
        print(self.a5[0])
        loss = - cp.sum(y * cp.log(self.a5)) / self.batch       # 交叉熵
        self.d5 = self.a5 - y                                   # bx10 , 用于反向传播

        return loss, self.a5

    def backward(self):
        dw5 = cp.dot(self.a4.T, self.d5) / self.batch       # 84 x 10
        db5 = cp.sum(self.d5, axis=0) / self.batch          # 10
        d4  = cp.dot(self.d5, self.w5.T) * Drelu(self.z4)   # b x 84
        dw4 = cp.dot(self.a3.T, d4) / self.batch            # 120 x 84
        db4 = cp.sum(d4, axis=0) / self.batch               # 84
        d3  = cp.dot(d4, self.w4.T) * Drelu(self.z3)        # b x 120
        dw3 = cp.dot(self.f0.T, d3) / self.batch            # 400 x 120
        db3 = cp.sum(d3, axis=0) / self.batch               # 120
        dp2 = cp.dot(d3, self.w3.T)                         # b x 400

        dp2 = dp2.reshape(self.batch, 16, 5, 5)             # bx16x5x5
        dp2 = pool_backward(dp2, self.mask2, self.z2)       # bx16x10x10
        dk2, db2, dp1 = conv_backward(self.p1, self.k2, dp2, stride=1)
        dp1 = pool_backward(dp1, self.mask1, self.z1)
        dk1, db1, _ = conv_backward(self.x, self.k1, dp1, stride=1)

        # print(self.w5[0])
        # print(dw5[0])
        self.k1 -= self.lr * dk1
        self.b1 -= self.lr * db1
        self.k2 -= self.lr * dk2
        self.b2 -= self.lr * db2
        self.w3 -= self.lr * dw3
        self.b3 -= self.lr * cp.squeeze(db3)
        self.w4 -= self.lr * dw4
        self.b4 -= self.lr * cp.squeeze(db4)
        self.w5 -= self.lr * dw5
        self.b5 -= self.lr * cp.squeeze(db5)
        # print(self.k1[0])
        # print(dk1[0])


def train():
    cnn = LeNet(batch=128, lr=0.01)
    for epoch in range(10):
        for i in range(0, 60000, cnn.batch):
            X = train_data.data[i:i+cnn.batch]
            if X.shape[0] != cnn.batch:
                break
            y = train_data.targets[i:i+cnn.batch]
            loss, _ = cnn.forward(X, y)
            print("Epoch:", epoch, "-", "{:6d}".format(i), ":", "{:.3f}".format(cp.asnumpy(loss)))
            cnn.backward()
        cp.savez("data2.npz", k1=cnn.k1, b1=cnn.b1, k2=cnn.k2, b2=cnn.b2, w3=cnn.w3, b3=cnn.b3, w4=cnn.w4, b4=cnn.b4, w5=cnn.w5, b5=cnn.b5)

def eval():
    r = cp.load("data2.npz")
    cnn = LeNet()
    cnn.k1 = r["k1"]
    cnn.b1 = r["b1"]
    cnn.k2 = r["k2"]
    cnn.b2 = r["b2"]
    cnn.w3 = r["w3"]
    cnn.b3 = r["b3"]
    cnn.w4 = r["w4"]
    cnn.b4 = r["b4"]
    cnn.w5 = r["w5"]
    cnn.b5 = r["b5"]
    acc_num = 0
    for i in range(0, 10000):
        _, result = cnn.forward(test_data.data[i], test_data.targets2[i])
        result = cp.argmax(result, axis=1)
        acc_num += result == test_data.targets[i]
    precison = acc_num / 10000
    print("Precison:", precison)

def conv_test():
    z = cp.random.randn(12, 1, 6, 6).astype(cp.float64)
    K = cp.random.randn(3, 1, 3, 3).astype(cp.float64)
    b = cp.zeros(3).astype(cp.float64)

    y_true = cp.ones((12, 3, 2, 2))

    for i in range(1000):
        # 前向
        next_z = conv_forward(z, K, b, stride=1, pad=0)
        pool_z, mask = pool_forward(relu(next_z))
        # 反向
        loss = cp.sum((pool_z - y_true) * (pool_z - y_true)) / 2
        dy = pool_z - y_true
        poll_d = pool_backward(dy, mask, next_z)
        dK, db, _ = conv_backward(z, K, poll_d, stride=1)
        # 更新梯度
        K -= 0.01 * dK
        b -= 0.01 * db

        # 打印损失
        print("step:{},loss:{}".format(i, loss))




if __name__ == '__main__':

    # Mnist手写数字集
    train_data = torchvision.datasets.MNIST(root='./mnist/', train=True, download=False)
    test_data = torchvision.datasets.MNIST(root='./mnist/', train=False)
    train_data.data = train_data.data.numpy()         # [60000,28,28]
    train_data.targets = train_data.targets.numpy()   # [60000]
    test_data.data = test_data.data.numpy()           # [10000,28,28]
    test_data.targets = test_data.targets.numpy()     # [10000]

    # 输入向量处理
    train_data.data = train_data.data.reshape(60000, 28*28)
    train_data.data = (train_data.data - np.mean(train_data.data)) / np.std(train_data.data)
    train_data.data = train_data.data.reshape(60000, 1, 28, 28)

    test_data.data = test_data.data.reshape(10000, 28 * 28)
    test_data.data = (test_data.data - np.mean(test_data.data)) / np.std(test_data.data)
    test_data.data = test_data.data.reshape(10000, 1, 28, 28)

    # 标签one-hot处理
    train_data.targets = onehot(train_data.targets, 60000) # (60000, 10)
    test_data.targets2 = onehot(test_data.targets, 10000)  # 测试时用于前向传播

    #conv_test()
    train()
    #eval()





