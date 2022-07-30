import numpy as np
import math

def get_H(result):
    H = 0
    un_result = np.unique(result)
    for i in un_result:
        p = 0
        for j in result:
            if j == i:
                p += 1
        proportion = p / len(result)
        H -= proportion * math.log(proportion, 2)
    return H

class Node:
    def __init__(self, leftchild, rightchild, divide, attribute, type, result):
        self.leftchild = leftchild
        self.rightchild = rightchild
        self.divide = divide
        self.attribute = attribute
        self.type = type
        self.result = result

class DecisionTree:
    def __init__(self):
        self.root = None

    def fit(self, train_features, train_labels):
        '''
        TODO: 实现决策树学习算法.
        train_features是维度为(训练样本数,属性数)的numpy数组
        train_labels是维度为(训练样本数, )的numpy数组
        '''

        def treegenerate(train_features, train_labels):
            train_labels = np.expand_dims(train_labels, axis=1)
            labels_features = np.concatenate((train_labels, train_features), axis=1)
            features_dim = np.shape(train_features)
            max_H = 0
            for feature in range(features_dim[1]):
                un_divide = np.unique(np.expand_dims(train_features[:, feature], axis=1))
                for divide in un_divide:
                    left = []
                    right = []
                    list = labels_features.tolist()
                    for element in list:
                        if element[feature + 1] > divide:
                            right.append(element)
                        else:
                            left.append(element)
                    leftgoal = np.array(left)
                    rightgoal = np.array(right)
                    if len(leftgoal) != 0 and len(rightgoal) != 0:
                        pro1 = len(leftgoal[:, 0]) / len(train_labels)
                        pro2 = len(rightgoal[:, 0]) / len(train_labels)
                        if max_H < get_H(train_labels) - pro1 * get_H(leftgoal[:, 0]) - pro2 * get_H(rightgoal[:, 0]):
                            max_H = get_H(train_labels) - pro1 * get_H(leftgoal[:, 0]) - pro2 * get_H(rightgoal[:, 0])
                            max_feature = feature
                            max_divide = divide
                            max_left = leftgoal
                            max_right = rightgoal
            # 叶子节点
            if max_H == 0:
                max_num = 0
                un_labels = np.unique(train_labels)
                for i in un_labels:
                    for j in train_labels:
                        num = 0
                        if j[0] == i:
                            num += 1
                    if max_num < num:
                        max_num = num
                        max_result = i
                return Node(None, None, None, None, True, max_result)
            # 中间节点
            else:
                leftchild = treegenerate(max_left[:, 1:], max_left[:, 0])
                rightchild = treegenerate(max_right[:, 1:], max_right[:, 0])
                return Node(leftchild, rightchild, max_divide, max_feature, False, None)

        self.root = treegenerate(train_features, train_labels)

    def predict(self, test_features):
        '''
        TODO: 实现决策树预测.
        test_features是维度为(测试样本数,属性数)的numpy数组
        该函数需要返回预测标签，返回一个维度为(测试样本数, )的numpy数组
        '''
        predict_result = []
        for feature in test_features:
            node = self.root
            while node.type != True:
                if feature[node.attribute] > node.divide:
                    node = node.rightchild
                else:
                    node = node.leftchild
            predict_result.append(node.result)
        # print(predict_result)
        return np.array(predict_result)

# treenode: [attr, feat[attr] == 1, feat[attr] == 2, feat[attr] == 3]
