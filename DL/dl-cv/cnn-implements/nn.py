import torchvision
import numpy as np

# 标签one-hot处理
def onehot(targets, num):
    result = np.zeros((num, 10))
    for i in range(num):
        result[i][targets[i]] = 1
    return result

# sigmoid
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

# sigmoid的一阶导数
def Dsigmoid(x):
    return sigmoid(x)*(1-sigmoid(x))


class NN(object):
    def __init__(self, l0, l1, l2):
        self.lr = 0.3  #学习率
        self.W1 = np.random.randn(l0, l1) * 0.01
        self.b1 = np.random.randn(l1) * 0.01
        self.W2 = np.random.randn(l1, l2) * 0.01
        self.b2 = np.random.randn(l2) * 0.01

    # 前向传播
    def forward(self, X, y):
        self.X = X                                           # m x 784
        self.z1 = np.dot(X, self.W1) + self.b1               # m x 500, 等于中间层层数
        self.a1 = sigmoid(self.z1)
        self.z2 = np.dot(self.a1, self.W2) + self.b2         # m x 10
        self.a2 = sigmoid(self.z2)
          #loss = np.sum((self.a2 - y) * (self.a2 - y)) / 2    # 均方差
          #self.d2 = (self.a2 - y) * Dsigmoid(self.z2)         # m x 10 , 用于反向传播
        self.a2 = np.exp(self.a2) / np.tile(np.sum(np.exp(self.a2), axis=1).reshape(3,1), 10)  # Softmax层，预测时改为(10000,1)
        loss = - np.sum(y * np.log(self.a2)) / 3                # 交叉熵
        self.d2 = (self.a2 - y) * Dsigmoid(self.z2)             # m x 10 , 用于反向传播
        return loss, self.a2

    # 反向传播
    def backward(self):
        dW2 = np.dot(self.a1.T, self.d2) / 3                  # 500 x 10, batchsize=3
        db2 = np.sum(self.d2, axis=0) / 3                     # 10
        d1 = np.dot(self.d2, self.W2.T) * Dsigmoid(self.z1)   # m x 500, 用于反向传播
        dW1 = np.dot(self.X.T, d1) / 3                        # 784x 500
        db1 = np.sum(d1, axis=0) / 3                          # 500

        self.W2 -= self.lr * dW2
        self.b2 -= self.lr * db2
        self.W1 -= self.lr * dW1
        self.b1 -= self.lr * db1



def train():
    nn = NN(784, 500, 10)

    for epoch in range(10):
        for i in range(0, 60000, 3):
            X = train_data.data[i:i + 3]
            y = train_data.targets[i:i + 3]
            loss, _ = nn.forward(X, y)
            print("Epoch:", epoch, "-", i, ":", "{:.3f}".format(loss))
            nn.backward()
            if i%10000==0:
                nn.lr -= nn.lr * 0.2


        np.savez("data.npz", w1=nn.W1, b1=nn.b1, w2=nn.W2, b2=nn.b2)

def test():
    r = np.load("data.npz")
    nn = NN(784, 500, 10)
    nn.W1 = r["w1"]
    nn.b1 = r["b1"]
    nn.W2 = r["w2"]
    nn.b2 = r["b2"]
    _, result = nn.forward(test_data.data, test_data.targets2)
    result = np.argmax(result, axis=1)
    precison = np.sum(result==test_data.targets) / 10000
    print("Precison:", precison)

if __name__ == '__main__':

    # Mnist手写数字集
    train_data = torchvision.datasets.MNIST(root='./mnist/', train=True, download=False)
    test_data = torchvision.datasets.MNIST(root='./mnist/', train=False)
    train_data.data = train_data.data.numpy()         # [60000,28,28]
    train_data.targets = train_data.targets.numpy()   # [60000]
    test_data.data = test_data.data.numpy()           # [10000,28,28]
    test_data.targets = test_data.targets.numpy()     # [10000]

    # 输入向量处理
    train_data.data = train_data.data.reshape(60000, 28 * 28) / 255.  # (60000, 784)
    test_data.data = test_data.data.reshape(10000, 28 * 28) / 255.

    # 标签one-hot处理
    train_data.targets = onehot(train_data.targets, 60000) # (60000, 10)
    test_data.targets2 = onehot(test_data.targets, 10000)  # 用于前向传播

    train()
    #test()





