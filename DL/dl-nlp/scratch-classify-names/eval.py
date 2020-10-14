import torch
from model import RNN
from config import *
from data import TrainData
from predict import categoryFromOutput

def eval(train_data, rnn):
    n_iters = 10000
    correct_sum = 0

    for i in range(n_iters):
        category, line, category_tensor, line_tensor = train_data.randomTrainingExample()

        output = rnn(line_tensor)

        guess, guess_i = categoryFromOutput(output)
        if guess == category:
            correct_sum += 1

    print("正确率：", correct_sum/n_iters)

## eval
if __name__ == '__main__':
    train_data = TrainData()

    rnn = RNN(N_LETTERS, 128, N_CATEGORIES)
    rnn.load_state_dict(torch.load("rnn.pth")) 

    eval(train_data, rnn)