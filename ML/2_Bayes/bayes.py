import numpy as np


###只适用于两分类问题，待修改###
#trainMatrix:   词向量矩阵
#trainCategory: 样本标签向量
def trainNB0(trainMatrix,trainCategory):
    numTrainDocs = len(trainMatrix)                     #词向量样本数
    numWords = len(trainMatrix[0])                      #词向量单词数（=特征数）
    pAbusive = sum(trainCategory)/float(numTrainDocs)   #标签为1的样本的概率
    p0Num = np.ones(numWords)                              #每个单词数初始化为1，防止条件概率出现0
    p1Num = np.ones(numWords)
    p0Denom = 2.0                                       #每个类别单词总数初始化为2
    p1Denom = 2.0
    for i in range(numTrainDocs):
        if trainCategory[i] == 1:
            p1Num += trainMatrix[i]
            p1Denom += sum(trainMatrix[i])
        else:
            p0Num += trainMatrix[i]
            p0Denom += sum(trainMatrix[i])
    p1Vect = np.log(p1Num/p1Denom)          #change to log() 防止下溢出
    p0Vect = np.log(p0Num/p0Denom)
    return p0Vect,p1Vect,pAbusive


def classifyNB(vec2Classify, p0Vec, p1Vec, pClass1):
    p1 = sum(vec2Classify * p1Vec) + np.log(pClass1)    #element-wise mult
    p0 = sum(vec2Classify * p0Vec) + np.log(1.0 - pClass1)
    if p1 > p0:
        return 1
    else: 
        return 0








