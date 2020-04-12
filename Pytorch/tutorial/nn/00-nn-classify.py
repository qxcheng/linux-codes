import torch
import torch.nn.functional as F     
import matplotlib.pyplot as plt

n_data = torch.ones(100, 2)         
x0 = torch.normal(2*n_data, 1)      # (100, 2)
y0 = torch.zeros(100)               # (100, )
x1 = torch.normal(-2*n_data, 1)     # (100, 2)
y1 = torch.ones(100)                # (100, )
x = torch.cat((x0, x1), 0).type(torch.FloatTensor)  # (200, 2)
y = torch.cat((y0, y1), ).type(torch.LongTensor)    # (200, )
plt.scatter(x.data.numpy(), y.data.numpy())
# plt.scatter(x.data.numpy()[:, 0], x.data.numpy()[:, 1], c=y.data.numpy(), s=100, lw=0, cmap='RdYlGn')
plt.show()

class Net(torch.nn.Module):             
    def __init__(self, n_feature, n_hidden, n_output):
        super(Net, self).__init__()     
        self.hidden = torch.nn.Linear(n_feature, n_hidden)   
        self.out = torch.nn.Linear(n_hidden, n_output)       

    def forward(self, x):
        x = F.relu(self.hidden(x))      
        x = self.out(x)                         # 输出值, 但不是预测值, 预测值需要另外计算
        return x

net = Net(n_feature=2, n_hidden=10, n_output=2) # output=类别数
print(net)  
optimizer = torch.optim.SGD(net.parameters(), lr=0.02)  # 传入net所有参数
loss_func = torch.nn.CrossEntropyLoss() # 误差不是one-hot 形式的, 而是1D Tensor: (batch,)
                                        # 预测值是2D tensor: (batch, n_classes)
plt.ion()   # 画图
plt.show()

for t in range(100):
    out = net(x)                 
    loss = loss_func(out, y)     
    optimizer.zero_grad()        
    loss.backward()              
    optimizer.step()            

    if t % 2 == 0:
        plt.cla()
        prediction = torch.max(F.softmax(out), 1)[1] # 通过softmax计算预测值
        pred_y = prediction.data.numpy().squeeze()
        target_y = y.data.numpy()
        plt.scatter(x.data.numpy()[:, 0], x.data.numpy()[:, 1], c=pred_y, s=100, lw=0, cmap='RdYlGn')
        accuracy = sum(pred_y == target_y)/200.      # 预测中有多少和真实值一样
        plt.text(1.5, -4, 'Accuracy=%.2f' % accuracy, fontdict={'size': 20, 'color':  'red'})
        plt.pause(0.1)

plt.ioff()  # 停止画图
plt.show()