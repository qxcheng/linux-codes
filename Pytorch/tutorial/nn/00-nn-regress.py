import torch
import torch.nn.functional as F                         
import matplotlib.pyplot as plt

x = torch.unsqueeze(torch.linspace(-1, 1, 100), dim=1)  # (100, 1)
y = x.pow(2) + 0.2*torch.rand(x.size())                 # (100, 1)
plt.scatter(x.data.numpy(), y.data.numpy())             # 画图
plt.show()

class Net(torch.nn.Module):             
    def __init__(self, n_feature, n_hidden, n_output):
        super(Net, self).__init__()     
        self.hidden = torch.nn.Linear(n_feature, n_hidden)   
        self.predict = torch.nn.Linear(n_hidden, n_output) 

        '''
		self.net = torch.nn.Sequential(
			torch.nn.Linear(1, 10),
			torch.nn.ReLU(),
			torch.nn.Linear(10, 1)
		)
		# forward: x = self.net(x)
		'''  
    def forward(self, x):               
        x = F.relu(self.hidden(x))      
        x = self.predict(x)             
        return x

net = Net(n_feature=1, n_hidden=10, n_output=1)
print(net)  
optimizer = torch.optim.SGD(net.parameters(), lr=0.2)  # 传入 net 所有参数
loss_func = torch.nn.MSELoss()                         # 均方差

plt.ion()                            # 画图
plt.show()

for epoch in range(100):
    prediction = net(x)              # 输入训练数据, 输出预测值
    loss = loss_func(prediction, y)  # 计算误差
    optimizer.zero_grad()            # 清空上一步的梯度
    loss.backward()                  # 误差反向传播
    optimizer.step()                 # 更新梯度
	'''
	torch.save(net, 'net.pkl')       # 保存整个网络
	net2 = torch.load('net.pkl')     # 提取
	prediction = net2(x)
	'''
	'''
    torch.save(net1.state_dict(), 'net_params.pkl')    # 保存参数
    net3 = torch.nn.Sequential(                        # 新建网络
        torch.nn.Linear(1, 10),
        torch.nn.ReLU(),
        torch.nn.Linear(10, 1)
    )
    net3.load_state_dict(torch.load('net_params.pkl')) # 加载参数
    prediction = net3(x)
    '''
	if epoch % 5 == 0:
        plt.cla()
        plt.scatter(x.data.numpy(), y.data.numpy())
        plt.plot(x.data.numpy(), prediction.data.numpy(), 'r-', lw=5)
        plt.text(0.5, 0, 'Loss=%.4f' % loss.data.numpy(), fontdict={'size': 20, 'color':  'red'})
        plt.pause(0.1)	
        