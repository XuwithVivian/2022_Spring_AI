import numpy as np
from cvxpy import *

class SupportVectorMachine:
    def __init__(self, C=1, kernel='Linear', epsilon=1e-4):
        self.C = C
        self.epsilon = epsilon
        self.kernel = kernel

        # Hint: 你可以在训练后保存这些参数用于预测
        # SV即Support Vector，表示支持向量，SV_alpha为优化问题解出的alpha值，
        # SV_label表示支持向量样本的标签。
        self.SV = None
        self.SV_alpha = None
        self.SV_label = None
        self.b = 0

    def KERNEL(self, x1, x2, d=2, sigma=1):
        # d for Poly, sigma for Gauss
        if self.kernel == 'Gauss':
            K = np.exp(-(np.sum((x1 - x2) ** 2)) / (2 * sigma ** 2))
        elif self.kernel == 'Linear':
            K = np.dot(x1,x2)
        elif self.kernel == 'Poly':
            K = (np.dot(x1,x2) + 1) ** d
        else:
            raise NotImplementedError()
        return K

    def fit(self, train_data, train_label):
        '''
        TODO：实现软间隔SVM训练算法
        train_data：训练数据，是(N, 7)的numpy二维数组，每一行为一个样本
        train_label：训练数据标签，是(N,)的numpy数组，和train_data按行对应
        '''
        def get_value(dim):
            m1 = np.outer(train_label, train_label) * kernel
            m2 = (-1) * np.ones(dim).T
            m3 = np.vstack((np.diag(np.ones(dim) * -1), np.diag(np.ones(dim))))
            if self.C is not None:
                m4 = np.hstack((np.zeros(dim), self.C * np.ones(dim)))
            else:
                m4 = np.zeros(dim)
            m5 = train_label.reshape(1, dim)
            vari = Variable(dim)
            obj = Minimize(quad_form(vari, m1) / 2 + m2 @ vari)
            prob = Problem(obj, [m5 @ vari == np.array([0]), m3 @ vari <= m4])
            prob.solve(solver=cvxpy.ECOS)
            value = np.ravel(vari.value)
            return value

        data_dim = np.shape(train_data)
        kernel = np.zeros((data_dim[0], data_dim[0]))
        for row in range(data_dim[0]):
            for column in range(data_dim[0]):
                kernel[row, column] = self.KERNEL(train_data[row,:], train_data[column,:])
        value = get_value(data_dim[0])
        list_SV = []
        list_alpha = []
        list_label = []
        for i in range(len(value)):
            if value[i] > self.epsilon:
                list_SV.append(train_data[i])
                list_alpha.append(value[i])
                list_label.append(train_label[i])
        self.b = list_label[0]
        for i in range(len(list_alpha)):
            self.b -= self.KERNEL(list_SV[i], list_SV[0]) * list_alpha[i] * list_label[i]
        self.SV = np.array(list_SV)
        self.SV_alpha = np.array(list_alpha)
        self.SV_label = np.array(list_label)

    def predict(self, test_data):
        '''
        TODO：实现软间隔SVM预测算法
        train_data：测试数据，是(M, 7)的numpy二维数组，每一行为一个样本
        必须返回一个(M,)的numpy数组，对应每个输入预测的标签，取值为1或-1表示正负例
        '''
        result = []
        for data in test_data:
            num = self.b
            for i in range(len(self.SV_alpha)):
                num += self.KERNEL(self.SV[i], data) * self.SV_alpha[i] * self.SV_label[i]
            result.append(np.sign(num))
        return np.array(result)
