import torch
from config import *
from model import RNN
from data import lineToTensor

# 获取预测结果
def categoryFromOutput(output):
    top_n, top_i = output.topk(1)
    category_i = top_i[0].item()
    return ALL_CATEGORIES[category_i], category_i

def predict(input_line, rnn, n_predictions=3):
    print('\n> %s' % input_line)

    with torch.no_grad():
        line_tensor = lineToTensor(input_line)

        output = rnn(line_tensor)

        # Get top N categories
        topv, topi = output.topk(n_predictions, 1, True)
        predictions = []

        for i in range(n_predictions):
            value = topv[0][i].item()
            category_index = topi[0][i].item()
            print('(%.2f) %s' % (value, ALL_CATEGORIES[category_index]))
            predictions.append([value, ALL_CATEGORIES[category_index]])


if __name__ == '__main__':
    rnn = RNN(N_LETTERS, 128, N_CATEGORIES)
    rnn.load_state_dict(torch.load("rnn.pth")) 

    # predict
    predict('Dovesky', rnn)
    predict('Jackson', rnn)
    predict('Satoshi', rnn)