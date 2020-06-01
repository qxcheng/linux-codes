import torch
from torch import nn
import torch.utils.data as Data
import torchvision
import torchvision.datasets as dsets
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
torch.manual_seed(1)          # reproducible


class RNN(nn.Module):
    def __init__(self):
        super(RNN, self).__init__()
        self.rnn = nn.LSTM(
            input_size=28,      # 图片每行的像素点数据
            hidden_size=64,     # rnn hidden unit
            num_layers=1,       # 有几层 RNN layers
            batch_first=True,   # input & output 以 batch size 为第一维度(batch, time_step, input_size)
        )
        self.out = nn.Linear(64, 10)    # 输出层

    def forward(self, x):
        # x : (batch, time_step, input_size)
        # r_out : (batch, time_step, output_size)
        # h_n shape (n_layers, batch, hidden_size)   LSTM 有两个 hidden states, h_n 是分线, h_c 是主线
        # h_c shape (n_layers, batch, hidden_size)
        r_out, (h_n, h_c) = self.rnn(x, None)   # None 表示 hidden state 会用全0的 state

        # 选取最后一个时间点的 r_out 输出
        # 这里 r_out[:, -1, :] 的值也是 h_n 的值
        out = self.out(r_out[:, -1, :])
        return out

def train(trainloader):
    pass


if __name__ == '__main__':
    EPOCH = 1
    BATCH_SIZE = 64
    TIME_STEP = 28         # rnn 时间步数 : 图片高度
    INPUT_SIZE = 28        # rnn 每步输入向量 : 图片每行像素
    LR = 0.01

    train_data = torchvision.datasets.MNIST(
        root='D:/datasets/mnist/',
        train=True,
        transform=torchvision.transforms.ToTensor(),
        download=False,
    )
    # (64, 1, 28, 28)
    train_loader = Data.DataLoader(dataset=train_data, batch_size=BATCH_SIZE, shuffle=True)

    test_data = torchvision.datasets.MNIST(root='D:/datasets/mnist/', train=False, download=False)
    # 只测试前2000个
    test_x = torch.unsqueeze(test_data.test_data, dim=1).type(torch.FloatTensor)[:2000]/255.   # shape from (2000, 28, 28) to (2000, 1, 28, 28), value in range(0,1)
    test_y = test_data.test_labels[:2000]

    rnn = RNN()
    optimizer = torch.optim.Adam(rnn.parameters(), lr=LR)
    loss_func = nn.CrossEntropyLoss()

    for epoch in range(EPOCH):
        for step, (x, b_y) in enumerate(train_loader):
            b_x = x.view(-1, 28, 28)        # reshape x to (batch, time_step, input_size)

            output = rnn(b_x)
            loss = loss_func(output, b_y)
            optimizer.zero_grad()           # clear gradients for this training step
            loss.backward()                 # backpropagation, compute gradients
            optimizer.step()                # apply gradients

    # 预测
    test_output = rnn(test_x[:10].view(-1, 28, 28))
    pred_y = torch.max(test_output, 1)[1].data.numpy().squeeze()
    print(pred_y, 'prediction number')
    print(test_y[:10], 'real number')
    """
    [7 2 1 0 4 1 4 9 5 9] prediction number
    [7 2 1 0 4 1 4 9 5 9] real number
    """



