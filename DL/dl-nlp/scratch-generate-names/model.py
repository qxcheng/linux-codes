import torch
import torch.nn as nn
from config import *

class _RNN(nn.Module):
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

class RNN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(RNN, self).__init__()
        self.hidden_size = hidden_size

        self.i2h = nn.Linear(N_CATEGORIES + input_size + hidden_size, hidden_size)
        self.i2o = nn.Linear(N_CATEGORIES + input_size + hidden_size, output_size)
        self.o2o = nn.Linear(hidden_size + output_size, output_size)
        self.dropout = nn.Dropout(0.1)
        self.softmax = nn.LogSoftmax(dim=1)

    def forward(self, category, input, hidden):
        input_combined = torch.cat((category, input, hidden), 1)
        hidden = self.i2h(input_combined)
        output = self.i2o(input_combined)
        output_combined = torch.cat((hidden, output), 1)
        output = self.o2o(output_combined)
        output = self.dropout(output)
        output = self.softmax(output)
        return output, hidden

    def initHidden(self):
        return torch.zeros(1, self.hidden_size)

        
if __name__ == '__main__':
    rnn = RNN(57, 128, 18)

    from data import lineToTensor
    x = lineToTensor('Albert')    # (6,1,57)

    output = rnn(x)
    print(output.shape) # (1, 18)
    print(output)