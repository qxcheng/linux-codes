import torch
import torch.nn as nn
import time
import math
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

from model import RNN
from data import TrainData
from config import *
from predict import categoryFromOutput


# 使用的时间
def timeSince(since):
    now = time.time()
    s = now - since
    m = math.floor(s / 60)
    s -= m * 60
    return '%dm %ds' % (m, s)


if __name__ == '__main__':
    train_data = TrainData()

    model = RNN(N_LETTERS, 128, N_CATEGORIES)

    criterion = nn.NLLLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.005)

    n_iters = 100000
    print_every = 5000
    plot_every = 1000
    
    current_loss = 0
    all_losses = []
    
    ## 训练
    start = time.time()
    for iter in range(1, n_iters + 1):
        category, line, category_tensor, line_tensor = train_data.randomTrainingExample()

        output = model(line_tensor)
        loss = criterion(output, category_tensor)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        current_loss += loss
        
        # Print iter number, loss, name and guess
        if iter % print_every == 0:
            guess, guess_i = categoryFromOutput(output)
            correct = 'T' if guess == category else 'F (%s)' % category
            print('%d %d%% (%s) %.4f %s / %s %s' % (iter, iter/n_iters*100, timeSince(start), loss, line, guess, correct))

        # Add current loss avg to list of losses
        if iter % plot_every == 0:
            all_losses.append(current_loss / plot_every)
            current_loss = 0

    torch.save(model.state_dict(), 'rnn.pth')        

    ## 画loss曲线图
    plt.figure()
    plt.plot(all_losses)

    
