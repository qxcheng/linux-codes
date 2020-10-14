import torch
import torch.nn as nn

class RNN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(RNN, self).__init__()

        self.batch_size = 1
        self.hidden_size = hidden_size
        self.initHidden()

        self.lstm = nn.LSTM(57, hidden_size, 2)
        self.linear= nn.Linear(hidden_size, output_size)
        self.softmax = nn.LogSoftmax(dim=1)

    def forward(self, x):
        output, _ = self.lstm(x, (self.h0, self.c0))
        output = output[-1,:,:]
        output = self.linear(output)
        output = self.softmax(output)
        return output

    def initHidden(self):
        self.h0 = torch.randn(2, self.batch_size, self.hidden_size)
        self.c0 = torch.randn(2, self.batch_size, self.hidden_size)


if __name__ == '__main__':
    rnn = RNN(57, 128, 18)

    from data import lineToTensor
    x = lineToTensor('Albert')    # (6,1,57)

    output = rnn(x)
    print(output.shape) # (1, 18)
    print(output)