import torch
import torchtext
from torchtext.datasets import text_classification
import os

if not os.path.isdir('./data'):
    os.mkdir('./data')

NGRAMS = 2
BATCH_SIZE = 16
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

train_dataset, test_dataset = text_classification.DATASETS['AG_NEWS'](
    root='./data', ngrams=NGRAMS, vocab=None)
