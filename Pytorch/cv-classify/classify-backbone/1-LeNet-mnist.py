import torch
import torch.nn as nn
import torch.nn.functional as F 
import torch.utils.data as Data
import torchvision              # 数据库模块
import matplotlib.pyplot as plt

class LeNet(nn.Module):
    def __init__(self):
        super(LeNet, self).__init__()                                      #mx1x28x28
        self.conv1 = nn.Conv2d(1, 6, kernel_size=5, stride=1, padding=2)   #mx6x28x28 -->mx6x14x14
        self.conv2 = nn.Conv2d(6, 16, kernel_size=5)                       #mx16x10x10-->mx16x5x5
        self.fc1   = nn.Linear(16*5*5, 120)  
        self.fc2   = nn.Linear(120, 84)
        self.fc3   = nn.Linear(84, 10)

    def forward(self, x): 
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2)) 
        x = F.max_pool2d(F.relu(self.conv2(x)), 2) 
        x = x.view(x.size()[0], -1)                     #mx400
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)        
        return x

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
#device = "cpu"    
torch.manual_seed(1)                              # reproducible     

EPOCH = 1                      
BATCH_SIZE = 50
LR = 0.001              
DOWNLOAD_MNIST = False 

train_data = torchvision.datasets.MNIST(
    root='./mnist/',   
    train=True,         
    transform=torchvision.transforms.ToTensor(), # 转换PIL.Image或numpy.ndarray成torch.FloatTensor(C x H x W), 训练的时候normalize到[0.0,1.0]区间                                               
    download=DOWNLOAD_MNIST,                        
)
test_data = torchvision.datasets.MNIST(
    root='./mnist/', 
    train=False, 
    transform=torchvision.transforms.ToTensor(),
    download=DOWNLOAD_MNIST, 
)
#train_data.data    [60000,28,28]
#train_data.targets [60000]
#test_data.data     [10000,28,28]
#test_data.targets  [10000]

# 批训练 50samples, 1 channel, 28x28 (50, 1, 28, 28)
train_loader = Data.DataLoader(dataset=train_data, batch_size=BATCH_SIZE, shuffle=True)
testloader = Data.DataLoader(dataset=test_data, batch_size=BATCH_SIZE, shuffle=False)

net = LeNet()
print(net)
optimizer = torch.optim.Adam(net.parameters(), lr=LR) 
loss_func = nn.CrossEntropyLoss()

for epoch in range(EPOCH):
    sum_loss = 0.
    for step, (b_x, b_y) in enumerate(train_loader):   # 分配 batch data, normalize x when iterate train_loader
        b_x, b_y = b_x.to(device), b_y.to(device)
        output = net(b_x)               # cnn output
        loss = loss_func(output, b_y)   # cross entropy loss
        optimizer.zero_grad()           # clear gradients for this training step
        loss.backward()                 # backpropagation, compute gradients
        optimizer.step()                # apply gradients

        sum_loss += loss.item()
        if step % 100 == 99:
            print('[Epoch:%d, Batch:%d] loss: %.03f' % (epoch + 1, step + 1, sum_loss / 100))
            sum_loss = 0.0           
    # 每跑完一次epoch测试一下准确率
    with torch.no_grad():
        correct = 0
        total = 0
        for data in testloader:
            images, labels = data
            images, labels = images.to(device), labels.to(device)
            outputs = net(images)
            # 取得分最高的那个类
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum()
        print('第%d个epoch的识别准确率为：%d%%' % (epoch + 1, (100 * correct / total)))
torch.save(net.state_dict(), 'net_params.pkl')

    

    