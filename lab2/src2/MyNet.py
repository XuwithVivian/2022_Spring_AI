import torch
import torchvision

import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

from torchvision import transforms


device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

class MyNet(nn.Module):
    def __init__(self):
        super(MyNet,self).__init__()
        ########################################################################
        #这里需要写MyNet的卷积层、池化层和全连接层
        self.conv_1 = nn.Conv2d(3, 16, (5, 5))
        self.conv_2 = nn.Conv2d(16, 32, (5, 5))
        self.relu = nn.ReLU()
        self.avg_pool = nn.AvgPool2d(2)
        self.linear_1 = nn.Linear(800, 120)
        self.linear_2 = nn.Linear(120, 84)
        self.linear_3 = nn.Linear(84, 10)

    def forward(self, x):
        ########################################################################
        #这里需要写MyNet的前向传播
        x = self.conv_1(x)
        x = self.relu(x)
        x = self.avg_pool(x)
        x = self.conv_2(x)
        x = self.relu(x)
        x = self.avg_pool(x)
        x = x.view(x.size(0), -1)
        x = self.linear_1(x)
        x = self.relu(x)
        x = self.linear_2(x)
        x = self.relu(x)
        x = self.linear_3(x)
        x = self.relu(x)
        return x

def train(net,train_loader,optimizer,n_epochs,loss_function):
    net.train()
    for epoch in range(n_epochs):
        for step, (inputs, labels) in enumerate(train_loader, start=0):
            # get the inputs; data is a list of [inputs, labels]
            inputs, labels = inputs.to(device), labels.to(device)
            
            ########################################################################
            #计算loss并进行反向传播
            optimizer.zero_grad()

            loss = loss_function(net(inputs), labels)
            loss.backward()

            optimizer.step()
            ########################################################################

            if step % 100 ==0:
                print('Train Epoch: {}/{} [{}/{}]\tLoss: {:.6f}'.format(
                    epoch, n_epochs, step * len(inputs), len(train_loader.dataset), loss.item()))

    print('Finished Training')
    save_path = './MyNet.pth'
    torch.save(net.state_dict(), save_path)

def test(net, test_loader, loss_function):
    net.eval()
    test_loss = 0.
    # num_correct = 0 #correct的个数
    with torch.no_grad():
        for inputs, labels in test_loader:
            inputs, labels = inputs.to(device), labels.to(device)
        ########################################################################
        #需要计算测试集的loss和accuracy
        test_loss = loss_function(net(inputs), labels)
        predict = net(inputs).max(1)[1]
        accuracy = torch.eq(predict, labels).float().mean()
        ########################################################################
        print("Test set: Average loss: {:.4f}\t Acc {:.2f}".format(test_loss.item(), accuracy))
    

if __name__ == '__main__':
    n_epochs =5
    train_batch_size = 128
    test_batch_size =5000 
    learning_rate = 5e-4

    transform = transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.247, 0.243, 0.261))])

    # 50000张训练图片
    train_set = torchvision.datasets.CIFAR10(root='./data', train=True,
                                             download=False, transform=transform)                                      
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=train_batch_size,
                                               shuffle=True, num_workers=0)

    # 10000张验证图片
    test_set = torchvision.datasets.CIFAR10(root='./data', train=False,
                                           download=False, transform=transform)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=test_batch_size,
                                             shuffle=False, num_workers=0)


    net = MyNet()

    # 自己设定优化器和损失函数
    # Adam类
    optimizer = torch.optim.Adam(net.parameters(), lr=learning_rate)
    # 交叉熵损失
    loss_function = torch.nn.CrossEntropyLoss()
    #######################################################################

    train(net,train_loader,optimizer,n_epochs,loss_function)
    test(net,test_loader,loss_function)


    
