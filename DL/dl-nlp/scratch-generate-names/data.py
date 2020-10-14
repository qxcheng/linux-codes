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


# category转换为One-hot张量
def categoryTensor(category):
    li = ALL_CATEGORIES.index(category)
    tensor = torch.zeros(1, N_CATEGORIES)
    tensor[0][li] = 1
    return tensor

# line转换为One-hot张量
def inputTensor(line):
    tensor = torch.zeros(len(line), 1, N_LETTERS)
    for li in range(len(line)):
        letter = line[li]
        tensor[li][0][ALL_LETTERS.find(letter)] = 1
    return tensor

# LongTensor of second letter to end (EOS) for target
def targetTensor(line):
    letter_indexes = [ALL_LETTERS.find(line[li]) for li in range(1, len(line))]
    letter_indexes.append(N_LETTERS - 1) # EOS
    return torch.LongTensor(letter_indexes)


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

    # 随机获取category和其中的一个随机line(name)
    def randomTrainingPair(self):
        category = randomChoice(ALL_CATEGORIES)
        line = randomChoice(self.category_lines[category])
        return category, line

    # 获取随机训练数据
    def randomTrainingExample(self):
        category, line = self.randomTrainingPair()
        category_tensor = categoryTensor(category)
        input_line_tensor = inputTensor(line)
        target_line_tensor = targetTensor(line)
        #print(category_tensor.shape, input_line_tensor.shape, target_line_tensor.shape)
        return category_tensor, input_line_tensor, target_line_tensor
        # (1, 18) (seq_len, 1, 57) (seq_len):第一个字母后面的字母的索引+EOS


if __name__ == "__main__": 
    data = TrainData()

    for i in range(10):
        category_tensor, input_line_tensor, target_line_tensor = data.randomTrainingExample()
        