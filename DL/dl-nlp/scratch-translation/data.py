from __future__ import unicode_literals, print_function, division
from io import open
import unicodedata
import string
import re
import random

import torch
import torch.nn as nn
from torch import optim
import torch.nn.functional as F

from config import * 


# unicode transfer to ascii
def unicodeToAscii(s):
    return ''.join(
        c for c in unicodedata.normalize('NFD', s)
        if unicodedata.category(c) != 'Mn'
    )

# Lowercase, trim, and remove non-letter characters
def normalizeString(s):
    s = unicodeToAscii(s.lower().strip())
    s = re.sub(r"([.!?])", r" \1", s)
    s = re.sub(r"[^a-zA-Z.!?]+", r" ", s)
    return s

# 读取文件返回翻译对
def readLangs(lang1, lang2, reverse=False):
    lines = open('data/%s-%s.txt' % (lang1, lang2), encoding='utf-8').read().strip().split('\n')

    # 分割每行为翻译对 并normalize
    pairs = [[normalizeString(s) for s in l.split('\t')] for l in lines]

    if reverse:
        # 法语到英语的翻译
        # pairs[['fra1', 'eng1'], [],...]
        pairs = [list(reversed(p)) for p in pairs]
        input_lang = Lang(lang2)   # 'fra'
        output_lang = Lang(lang1)  # 'eng'  
    else:
        input_lang = Lang(lang1)
        output_lang = Lang(lang2)

    return input_lang, output_lang, pairs


class Lang:
    def __init__(self, name):
        self.name = name
        self.word2index = {}
        self.word2count = {}
        self.index2word = {0: "SOS", 1: "EOS"}
        self.n_words = 2  # Count SOS and EOS

    def addSentence(self, sentence):
        for word in sentence.split(' '):
            self.addWord(word)

    def addWord(self, word):
        if word not in self.word2index:
            self.word2index[word] = self.n_words
            self.word2count[word] = 1
            self.index2word[self.n_words] = word
            self.n_words += 1
        else:
            self.word2count[word] += 1


# 过滤长度、句首
def filterPair(p):
    return len(p[0].split(' ')) < MAX_LENGTH and \
        len(p[1].split(' ')) < MAX_LENGTH and \
        p[1].startswith(eng_prefixes)

def filterPairs(pairs):
    return [pair for pair in pairs if filterPair(pair)]


def prepareData(lang1, lang2, reverse=False):
    input_lang, output_lang, pairs = readLangs(lang1, lang2, reverse)
    print("Read %s sentence pairs" % len(pairs))
    pairs = filterPairs(pairs)
    print("Trimmed to %s sentence pairs" % len(pairs))
    print("Counting words...")
    for pair in pairs:
        input_lang.addSentence(pair[0])
        output_lang.addSentence(pair[1])
    print("Counted words:")
    print(input_lang.name, input_lang.n_words)
    print(output_lang.name, output_lang.n_words)
    return input_lang, output_lang, pairs


def indexesFromSentence(lang, sentence):
    return [lang.word2index[word] for word in sentence.split(' ')]

def tensorFromSentence(lang, sentence):
    indexes = indexesFromSentence(lang, sentence)
    indexes.append(EOS_token)
    return torch.tensor(indexes, dtype=torch.long, device=device).view(-1, 1)


class TrainDatasets:
    def __init__(self):
        self.input_lang, self.output_lang, self.pairs = prepareData('eng', 'fra', True)

    def tensorsFromPair(self, pair):
        input_tensor = tensorFromSentence(self.input_lang, pair[0])
        target_tensor = tensorFromSentence(self.output_lang, pair[1])
        return (input_tensor, target_tensor)


if __name__ == '__main__':
    train_data = TrainDatasets()
    input_tensor, target_tensor = train_data.tensorsFromPair(train_data.pairs[20])
    print(train_data.pairs[20])
    print(input_tensor.shape, target_tensor.shape) # torch.Size([5, 1]) torch.Size([5, 1])
    print(input_tensor, target_tensor)

    

