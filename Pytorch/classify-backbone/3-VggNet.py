import torch
import torch.nn as nn
import torch.nn.functional as F 
import torch.utils.data as Data
import torchvision              
from torch.autograd import Variable 
import matplotlib.pyplot as plt
from tensorboardX import SummaryWriter

def vgg_block(num_convs, in_channels, out_channels):
    net = [nn.Conv2d(in_channels, out_channels, kernel_size=3, padding=1), nn.ReLU(True)] # 定义第一层
    for i in range(num_convs-1):   # 定义后面的很多层
        net.append(nn.Conv2d(out_channels, out_channels, kernel_size=3, padding=1))
        net.append(nn.ReLU(True))
    net.append(nn.MaxPool2d(2, 2)) # 定义池化层
    return nn.Sequential(*net)

class VGG16(nn.Module):
    def __init__(self):
        super(VGG16, self).__init__()
        self.block1 = vgg_block(2, 3, 64)
        self.block2 = vgg_block(2, 64, 128)
        self.block3 = vgg_block(2, 128, 256)
        self.block4 = vgg_block(3, 256, 512)
        self.block5 = vgg_block(3, 512, 512)
        self.fc = nn.Sequential(
            nn.Linear(512*7*7, 4096),
            nn.ReLU(True),
            nn.Linear(4096, 1000)
        )
    def forward(self, x):
        x = self.block1(x)
        x = self.block2(x)
        x = self.block3(x)
        x = self.block4(x)
        x = self.block5(x)
        x = x.view(x.shape[0], -1)
        x = self.fc(x)
        return x

class VGG19(nn.Module):
    def __init__(self):
        super(VGG19, self).__init__()
        self.block1 = vgg_block(2, 3, 64)
        self.block2 = vgg_block(2, 64, 128)
        self.block3 = vgg_block(2, 128, 256)
        self.block4 = vgg_block(4, 256, 512)
        self.block5 = vgg_block(4, 512, 512)
        self.fc = nn.Sequential(
            nn.Linear(512*7*7, 4096),
            nn.ReLU(True),
            nn.Linear(4096, 1000)
        )
    def forward(self, x):
        x = self.block1(x)
        x = self.block2(x)
        x = self.block3(x)
        x = self.block4(x)
        x = self.block5(x)
        x = x.view(x.shape[0], -1)
        x = self.fc(x)
        return x

x = Variable(torch.randn(1,3,224,224)) #change 12 to the channel number of network input
net =  VGG19()
with SummaryWriter(comment='VGGNet') as w:
    w.add_graph(net, (x,))

