from __future__ import unicode_literals, print_function, division
from io import open
import glob
import os
import unicodedata
import random
import torch
from config import *


# Unicode转ASCII, https://stackoverflow.com/a/518232/2809427
def unicodeToAscii(all_letters, s):
    return ''.join(
        c for c in unicodedata.normalize('NFD', s)
        if unicodedata.category(c) != 'Mn'
        and c in all_letters
    )

# 读取names.txt文件分行并作编码转换
def readLines(filename):
    lines = open(filename, encoding='utf-8').read().strip().split('\n')
    return [unicodeToAscii(ALL_LETTERS, line) for line in lines]


# 获取letter在all_letters中的索引, e.g. "a" = 0
def letterToIndex(letter):
    return ALL_LETTERS.find(letter)

# 将letter 转换为 <1 x n_letters> Tensor
def letterToTensor(letter):
    tensor = torch.zeros(1, N_LETTERS)
    tensor[0][letterToIndex(letter)] = 1
    return tensor
    # tensor([[0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
    #          0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 1.,
    #          0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
    #          0., 0., 0.]])

# 将line(name) 转换为 <name_length x 1 x n_letters>
def lineToTensor(line):
    tensor = torch.zeros(len(line), 1, N_LETTERS)
    for li, letter in enumerate(line):
        tensor[li][0][letterToIndex(letter)] = 1
    return tensor  # line='Jones'时为[5, 1, 57]


# 随机选择
def randomChoice(l):
    return l[random.randint(0, len(l) - 1)]


class TrainData(object):
    def __init__(self):
        self.category_lines = {} # {language: [names ...]}

        for filename in glob.glob('data/names/*.txt'):
            category = os.path.splitext(os.path.basename(filename))[0]
            lines = readLines(filename)
            self.category_lines[category] = lines

    # 获取随机训练数据
    def randomTrainingExample(self):
        category = randomChoice(ALL_CATEGORIES)
        line = randomChoice(self.category_lines[category])
        category_tensor = torch.tensor([ALL_CATEGORIES.index(category)], dtype=torch.long)
        line_tensor = lineToTensor(line)
        return category, line, category_tensor, line_tensor
        # Chinese Shao (1)即0-17的一个数 (4, 1, 57)


if __name__ == "__main__": 
    data = TrainData()
    print(letterToTensor('J'))
    print(lineToTensor('Jones').size())

    for i in range(10):
        category, line, category_tensor, line_tensor = data.randomTrainingExample()
        print('category =', category, '/ line =', line, category_tensor,
        line_tensor.shape)