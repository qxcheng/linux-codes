import torch
import torch.nn as nn
import torch.nn.functional as F 
import torch.utils.data as Data
import torchvision
from torch.autograd import Variable
import torchvision.transforms as transforms
import torch.optim as optim
import matplotlib.pyplot as plt
#from tensorboardX import SummaryWriter
import time

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
                nn.Conv2d(inchannel, outchannel, kernel_size=1, stride=stride, bias=False),
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
    def __init__(self, resnet_type='resnet18', num_classes=10):
        super(ResNet, self).__init__()
        self.img_size = 32
        if self.img_size == 224:
            #for 1x3x224x224
            self.conv1=nn.Sequential(
                nn.Conv2d(3,64,kernel_size=7,stride=2,padding=3,bias=False), #1x64x112x112
                nn.BatchNorm2d(64),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(kernel_size=3, stride=2, padding=1)             #1x64x56x56
            )
        elif self.img_size == 32:
            #for cifar10 32x32
            self.conv1 = nn.Sequential(
                nn.Conv2d(3, 64, kernel_size=3, stride=1, padding=1, bias=False),
                nn.BatchNorm2d(64),
                nn.ReLU(inplace=True),
            )

        if resnet_type == 'resnet18':
            self.layer1 = self.make_layer(BasicBlock, 64, 64, 2, stride=1)
            self.layer2 = self.make_layer(BasicBlock, 64, 128, 2, stride=2)
            self.layer3 = self.make_layer(BasicBlock, 128, 256, 2, stride=2)
            self.layer4 = self.make_layer(BasicBlock, 256, 512, 2, stride=2)
            self.fc = nn.Linear(512, num_classes)

        elif resnet_type == 'resnet34':
            self.layer1 = self.make_layer(BasicBlock, 64, 64, 3, stride=1)
            self.layer2 = self.make_layer(BasicBlock, 64, 128, 4, stride=2)
            self.layer3 = self.make_layer(BasicBlock, 128, 256, 6, stride=2)
            self.layer4 = self.make_layer(BasicBlock, 256, 512, 3, stride=2)
            self.fc = nn.Linear(512, num_classes)

        elif resnet_type == 'resnet50':
            self.layer1 = self.make_layer(BottleBlock, 64, 256, 3, stride=1)
            self.layer2 = self.make_layer(BottleBlock, 256, 512, 4, stride=2)
            self.layer3 = self.make_layer(BottleBlock, 512, 1024, 6, stride=2)
            self.layer4 = self.make_layer(BottleBlock, 1024, 2048, 3, stride=2)
            self.fc = nn.Linear(2048, num_classes)

        elif resnet_type == 'resnet101':
            self.layer1 = self.make_layer(BottleBlock, 64, 256, 3, stride=1)
            self.layer2 = self.make_layer(BottleBlock, 256, 512, 4, stride=2)
            self.layer3 = self.make_layer(BottleBlock, 512, 1024, 23, stride=2)
            self.layer4 = self.make_layer(BottleBlock, 1024, 2048, 3, stride=2)
            self.fc = nn.Linear(2048, num_classes)

        elif resnet_type == 'resnet152':
            self.layer1 = self.make_layer(BottleBlock, 64, 256, 3, stride=1)
            self.layer2 = self.make_layer(BottleBlock, 256, 512, 8, stride=2)
            self.layer3 = self.make_layer(BottleBlock, 512, 1024, 36, stride=2)
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
        out = self.layer1(out)
        out = self.layer2(out)
        out = self.layer3(out)
        out = self.layer4(out)
        #print(out.shape)
        if self.img_size == 224:
            out = F.avg_pool2d(out, 7)   # for 224x224
        elif self.img_size == 32:
            out = F.avg_pool2d(out, 4)   # for 32x32
        out = out.view(out.size(0), -1)
        out = self.fc(out)
        return out

def train(trainloader):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    net = ResNet(resnet_type='resnet18', num_classes=10).to(device)
    #print(net)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

    for epoch in range(10):
        start = time.time()
        sum_loss = 0.0
        correct = 0.0
        total = 0.0
        for i, data in enumerate(trainloader, 0):
            inputs, labels = data[0].to(device), data[1].to(device)
            #print(inputs.shape, labels.shape)
            #time.sleep(60)
            optimizer.zero_grad()
            outputs = net(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            sum_loss += loss.item()
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += predicted.eq(labels.data).cpu().sum()
            print('[epoch:%d, iter:%d] Loss: %.03f | Acc: %.3f%%'
                  % (epoch + 1, (i + 1), sum_loss / (i + 1), 100. * correct / total))
        print("One epoch time: ", time.time() - start)
    print('Finished Training')
    torch.save(net.state_dict(), "resnet18-cifar10.pth")

if __name__ == '__main__':
    ## cifar数据集加载
    transform = transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    trainset = torchvision.datasets.CIFAR10(root='D:/datasets', train=True,
                                            download=True, transform=transform)
    trainloader = torch.utils.data.DataLoader(trainset, batch_size=128,
                                              shuffle=True, num_workers=2)

    testset = torchvision.datasets.CIFAR10(root='D:/datasets', train=False,
                                           download=True, transform=transform)
    testloader = torch.utils.data.DataLoader(testset, batch_size=128,
                                             shuffle=False, num_workers=2)

    # with SummaryWriter(comment='RESNet') as w:
    #     w.add_graph(net, (x,))

    train(trainloader)

