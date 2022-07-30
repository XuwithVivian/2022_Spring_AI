import torch
import numpy as np
from matplotlib import pyplot as plt

class MLP:
    class Layer():
        def __init__(self, type, size_1 = 0, size_2 = 0):
            # 若为True，则存储每层之间待执行前向传播计算的数据；若为False，则存储每层之间待执行激活函数的数据
            self.type = type
            # 分别表示相邻层的size
            self.size_1 = size_1
            self.size_2 = size_2
            # 储存上一层的前向传播的值
            self.back_value = None
            # 定义W、b
            if type == True:
                scale = np.sqrt(size_1 / 2)
                self.w = np.random.randn(size_1, size_2) / scale
                self.b = np.random.randn(1, size_2) / scale
                # self.b = np.zeros((1, size_2))

    def __init__(self):
        # layer size = [10, 8, 8, 4]
        # 初始化所需参数
        self.layer_size = [10, 10, 8, 8, 4]
        self.epochs = 1500
        self.lr = 0.01
        self.all_layer = []
        for i in range(len(self.layer_size) - 1):
            if i > len(self.layer_size) - 3:
                self.all_layer.append(self.Layer(True, self.layer_size[i], self.layer_size[i + 1]))
            else:
                self.all_layer.append(self.Layer(True, self.layer_size[i], self.layer_size[i + 1]))
                self.all_layer.append(self.Layer(False))

    def forward(self, x):
        # 前向传播
        for i in self.all_layer:
            if i.type == True:
                i.back_value = x
                x = np.add(np.dot(x, i.w), i.b)
            else:
                i.back_value = x
                x = np.tanh(x)
        return x

    def backward(self, lr, inputs, labels):  # 自行确定参数表
        # 反向传播
        def get_loss(labels, forward_result):
            subtract_result = np.subtract(forward_result, np.max(forward_result, axis=1, keepdims=True))
            exp_result = np.exp(subtract_result)
            divide_result = np.divide(exp_result, np.sum(exp_result, axis=1, keepdims=True))
            list = []
            for i in range(train_batch_size):
                list.append(i)
            positive_pre = divide_result[np.array(list), labels]
            loss = -np.log(positive_pre)
            return np.mean(loss), divide_result

        def update_Wb(list_1, list_2, list_3, list_4):
            for i in range(len(list_1)):
                list_1[i] -= lr * list_3[i]
                list_2[i] -= lr * list_4[i]

        forward_result = self.forward(inputs)
        loss, new_result = get_loss(labels, forward_result)
        forgrad = np.zeros_like(new_result)
        list = []
        for i in range(train_batch_size):
            list.append(i)
        forgrad[np.array(list), labels] = 1.0
        list_1 = []
        list_2 = []
        list_3 = []
        list_4 = []
        grad = (new_result - forgrad) / train_batch_size
        for i in reversed(self.all_layer):
            if i.type == True:
                list_1.append(i.w)
                list_2.append(i.b)
                list_3.append(np.matmul(i.back_value.T, grad))
                list_4.append(np.sum(grad, axis=0, keepdims=True))
                grad = np.matmul(grad, i.w.T)
            else:
                grad = (1 - np.tanh(i.back_value) ** 2) * grad
        update_Wb(list_1, list_2, list_3, list_4)
        return loss

class MLPbyTorch(torch.nn.Module):
    def __init__(self):
        super(MLPbyTorch, self).__init__()
        self.linear_1 = torch.nn.Linear(10, 10)
        self.linear_2 = torch.nn.Linear(10, 8)
        self.linear_3 = torch.nn.Linear(8, 8)
        self.linear_4 = torch.nn.Linear(8, 4)
        self.tanh = torch.nn.Tanh()
        self.epochs = 1500
        self.lr = 0.01
        self.loss_function = torch.nn.CrossEntropyLoss()

    def forward(self, x):
        x = self.linear_1(x)
        x = self.tanh(x)
        x = self.linear_2(x)
        x = self.tanh(x)
        x = self.linear_3(x)
        x = self.tanh(x)
        x = self.linear_4(x)
        return x

def print_Wb(mlp: MLP):
    for i in range(1, len(mlp.layer_size)):
        print("第", i, "层W:")
        print(mlp.all_layer[2 * (i - 1)].w)
        print("第", i, "层b:")
        print(mlp.all_layer[2 * (i - 1)].b)

def train(mlp: MLP, epochs, lr, inputs, labels):
    """
        mlp: 传入实例化的MLP模型
        epochs: 训练轮数
        lr: 学习率
        inputs: 生成的随机数据
        labels: 生成的one-hot标签
    """
    train_loss = []
    dim_input = np.shape(inputs)
    num = 0
    avg_loss = 0.0
    for i in range(epochs):
        randlist = np.random.permutation(dim_input[0])
        begin = 0
        while begin < dim_input[0]:
            if dim_input[0] > begin + train_batch_size:
                pos = randlist[begin:begin + train_batch_size]
            else:
                pos = randlist[begin:dim_input[0]]
            num += 1
            begin += train_batch_size
            labels_idx = np.argmax(labels, axis=1)
            avg_loss += mlp.backward(lr, inputs[pos], labels_idx[pos])
            if num % 10 == 0:
                train_loss.append(avg_loss / 10)
                avg_loss = 0.0
    print_Wb(mlp)
    return train_loss

# 用torch训练
def train_bytorch(mlp: MLPbyTorch, epochs, lr, inputs, labels):
    mlp.train()
    train_loss = []
    dim_input = np.shape(inputs)
    optimizer = torch.optim.SGD(mlp.parameters(), lr=lr)
    num = 0
    avg_loss = 0.0
    for i in range(epochs):
        randlist = np.random.permutation(dim_input[0])
        begin = 0
        while begin < dim_input[0]:
            if dim_input[0] > begin + train_batch_size:
                pos = randlist[begin:begin + train_batch_size]
            else:
                pos = randlist[begin:dim_input[0]]
            num += 1
            begin += train_batch_size
            labels_idx = np.argmax(labels, axis=1)
            optimizer.zero_grad()
            tensor_return = mlp(torch.tensor(inputs[pos], dtype=torch.float32))
            loss = mlp.loss_function(tensor_return, torch.tensor(labels_idx[pos], dtype=torch.long))
            loss.backward()
            optimizer.step()
            avg_loss += loss.item()
            if num % 10 == 0:
                train_loss.append(avg_loss / 10)
                avg_loss = 0.0
    return train_loss

if __name__ == '__main__':
    # 设置随机种子,保证结果的可复现性
    np.random.seed(1)
    mlp = MLP()
    # 生成数据
    inputs = np.random.randn(100, 10)

    # 生成one-hot标签
    labels = np.eye(4)[np.random.randint(0, 4, size=(1, 100))].reshape(100, 4)
    train_batch_size = 10
    # 训练
    train_loss = train(mlp, mlp.epochs, mlp.lr, inputs, labels)
    # 以下代码用于对比torch，需对比时取消注释

    # mlp2 = MLPbyTorch()
    # torch_loss = train_bytorch(mlp2, mlp2.epochs, mlp2.lr, inputs, labels)
    # x = range(1, len(train_loss) + 1)
    # plt.title('loss-train')
    # plt.xlabel('train')
    # plt.ylabel('loss')
    # plt.plot(x, train_loss, label='bytrain')
    # plt.plot(x, torch_loss, label='bytorch')
    # plt.legend()
    # plt.show()