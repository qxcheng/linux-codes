import torch
import torch.nn as nn
import torch.nn.functional as F 
import torch.utils.data as Data
import torchvision              # 数据库模块
from torch.autograd import Variable # 变量
import matplotlib.pyplot as plt
from tensorboardX import SummaryWriter

# 虚线箭头处特征图尺寸减半，每层第一个残差块通道处发生变化

class BasicBlock(nn.Module):
    def __init__(self, inchannel, outchannel, stride=1):
        super(BasicBlock, self).__init__()
        self.left = nn.Sequential(
            nn.Conv2d(inchannel, outchannel, kernel_size=3, stride=stride, padding=1, bias=False),
            nn.BatchNorm2d(outchannel),
            nn.ReLU(inplace=True),
            nn.Conv2d(outchannel, outchannel, kernel_size=3, stride=1, padding=1, bias=False),
            nn.BatchNorm2d(outchannel)
        )
        self.shortcut = nn.Sequential()
        if stride != 1 or inchannel != outchannel:
            self.shortcut = nn.Sequential(
                nn.Conv2d(inchannel, inchannel, kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(outchannel)
            )

    def forward(self, x):
        out = self.left(x)
        out += self.shortcut(x)
        out = F.relu(out)
        return out

class BottleBlock(nn.Module):
    def __init__(self, inchannel, outchannel, stride=1):
        super(BottleBlock, self).__init__()
        if inchannel == outchannel:
            midchannel = inchannel // 4
        else:
            if inchannel == 64:
                midchannel = 64
            else:
                midchannel = inchannel // 2
        self.left = nn.Sequential(
            nn.Conv2d(inchannel, midchannel, kernel_size=1, stride=1, bias=False),
            nn.BatchNorm2d(midchannel),
            nn.Conv2d(midchannel, midchannel, kernel_size=3, stride=stride, padding=1, bias=False),
            nn.BatchNorm2d(midchannel),
            nn.Conv2d(midchannel, outchannel, kernel_size=1, stride=1, bias=False),
            nn.BatchNorm2d(outchannel),
            nn.ReLU(inplace=True),
        )
        self.shortcut = nn.Sequential()
        if stride != 1 or inchannel != outchannel:
            self.shortcut = nn.Sequential(
                nn.Conv2d(inchannel, outchannel, kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(outchannel)
            )

    def forward(self, x):
        out = self.left(x)
        out += self.shortcut(x)
        out = F.relu(out)
        return out

class ResNet(nn.Module):
    def __init__(self, BasicBlock, num_classes=10):
        super(ResNet, self).__init__()
        #1x3x224x224
        self.conv1=nn.Sequential(
            nn.Conv2d(3,64,kernel_size=7,stride=2,padding=3,bias=False), #1x64x112x112
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=3, stride=2, padding=1) #1x64x56x56
        )
        '''
        ResNet18: 2,2,2,2个residual block 
        ResNet34: 3,4,6,3个residual block 
        '''     
        # self.layer1 = self.make_layer(BasicBlock,  64,  64, 3, stride=1)
        # self.layer2 = self.make_layer(BasicBlock,  64, 128, 4, stride=2)
        # self.layer3 = self.make_layer(BasicBlock, 128, 256, 6, stride=2)
        # self.layer4 = self.make_layer(BasicBlock, 256, 512, 3, stride=2)
        # self.fc = nn.Linear(512, num_classes)
        '''
        RESNET50: 3,4,6,3
        RESNET101: 3,4,23,3
        RESNET152: 3,8,36,3
        '''
        self.layer1 = self.make_layer(BottleBlock,   64, 256,  3, stride=1)
        self.layer2 = self.make_layer(BottleBlock,  256, 512,  4, stride=2)
        self.layer3 = self.make_layer(BottleBlock,  512, 1024, 23, stride=2)
        self.layer4 = self.make_layer(BottleBlock, 1024, 2048, 3, stride=2)
        self.fc = nn.Linear(2048, num_classes)

    def make_layer(self, block, inchannel, outchannel, num_blocks, stride):
        layers = []
        layers.append(block(inchannel,outchannel,stride))
        for i in range(1,num_blocks):
            layers.append(block(outchannel,outchannel))
        return nn.Sequential(*layers)

    def forward(self, x):
        out = self.conv1(x)
        print(out.shape)
        out = self.layer1(out)
        print(out.shape)
        out = self.layer2(out)
        print(out.shape)
        out = self.layer3(out)
        print(out.shape)
        out = self.layer4(out)
        print(out.shape)
        out = F.avg_pool2d(out, 7)
        out = out.view(out.size(0), -1)
        out = self.fc(out)
        return out



x = Variable(torch.randn(1,3,224,224)) #change 12 to the channel number of network input
net = ResNet(BottleBlock)
# with SummaryWriter(comment='RESNet') as w:
#     w.add_graph(net, (x,))

net(x)