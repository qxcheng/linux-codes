#!/usr/bin/python
# -*- coding: utf-8 -*-

from math import log
import operator
import pickle

#计算给定数据集的香农熵
#数据集格式要求 m x n ，且最后一列为标签
def calcShannonEnt(dataset):
    #统计每个标签的样本数
    numEntries = len(dataset)
    labelCounts = {}
    for featVec in dataset:
        currentLabel = featVec[-1]
        if currentLabel not in labelCounts.keys():
            labelCounts[currentLabel] = 0
        labelCounts[currentLabel] += 1
    #计算香农熵
    shannonEnt = 0.0
    for key in labelCounts:
        prob = float(labelCounts[key])/numEntries
        shannonEnt -= prob*log(prob,2)
    return shannonEnt

#按照给定特征划分数据集
#给定数据集指定特征所在轴，特征值为value的样本会被提取到结果集，且该列被移除，表明该特征已使用
def splitDataset(dataSet, axis, value):
    retDataset = []
    for featVec in dataSet:
        if featVec[axis] == value:
            reducedFeatVec = featVec[:axis]
            reducedFeatVec.extend(featVec[axis+1:])
            retDataset.append(reducedFeatVec)
    return retDataset

#选择划分数据集的最好特征
def chooseBestFeatureToSplit(dataSet):
    numFeatures = len(dataSet[0]) - 1                  # 特征数
    baseEntropy = calcShannonEnt(dataSet)              # 初始数据集的熵
    bestInfoGain = 0.0;                                # 最大的信息增益
    bestFeature = -1                                   # 最好的特征的下标
    for i in range(numFeatures):
        featList = [example[i] for example in dataSet]
        uniqueVals = set(featList)                     # 得到第i列特征的所有值
        newEntropy = 0.0                               # 分类后的熵
        # 根据不同的value值，得到每个value子集，并计算子集的总熵
        for value in uniqueVals:
            subDataSet = splitDataset(dataSet, i, value)
            prob = len(subDataSet)/float(len(dataSet))
            newEntropy += prob * calcShannonEnt(subDataSet)  # 子集熵 x 子集概率
        infoGain = baseEntropy - newEntropy                  # 初始熵-分类后熵，计算信息增益，分类应会变得有序，导致总熵减少，因此此值越大越好
        if (infoGain > bestInfoGain):
            bestInfoGain = infoGain
            bestFeature = i
    return bestFeature

#用多数表决法处理特征值用完时标签不唯一的情况
def majorityCount(classList):
    classCount = {}
    for vote in classList:
        if vote not in classCount.keys():
            classCount[vote] = 0
        classCount += 1
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]

#创建决策树
#dataSet :数据集，包含标签列
#labels  :特征对应名字集
def createTree(dataSet,labels):
    classList = [example[-1] for example in dataSet]    # 标签集
    if classList.count(classList[0]) == len(classList): # 所有标签一样时，递归结束
        return classList[0]
    if len(dataSet[0]) == 1:                            # 特征用完时，多数表决，递归结束
        return majorityCount(classList)
    bestFeat = chooseBestFeatureToSplit(dataSet)        # 选择最优的特征
    bestFeatLabel = labels[bestFeat]                    # 最优特征的名字
    myTree = {bestFeatLabel:{}}                         # 决策树第一层
    del(labels[bestFeat])
    featValues = [example[bestFeat] for example in dataSet]
    uniqueValues = set(featValues)                             # 最优特征的所有值
    for value in uniqueValues:                                 # 对每个特征值递归创建决策树
        subLabels = labels[:]                                  # 复制列表，防止修改上层列表，导致索引错位
        myTree[bestFeatLabel][value] = createTree(splitDataset(dataSet, bestFeat, value), subLabels)
    return myTree

#使用决策树进行分类
#inputTree  :决策树字典
#featLabels :标签名字
#testVec    :特征向量
def classify(inputTree,featLabels,testVec):
    firstStr = list(inputTree.keys())[0]                  # 决策树第一个特征的名字
    secondDict = inputTree[firstStr]                      # 决策树第一个特的字典
    featIndex = featLabels.index(firstStr)                # 第一个特征在特征列表的索引
    for key in secondDict.keys():
        if testVec[featIndex] == key:                     # 待预测特征向量第一个特征的值
            if type(secondDict[key]).__name__ == 'dict':  # 值是字典则递归调用
                classLabel = classify(secondDict[key],featLabels,testVec)
            else:
                classLabel = secondDict[key]              # 值是标签则直接返回该标签
    return classLabel

#保存决策树
def storeTree(inputTree, filename):
    fw = open(filename, 'wb')
    pickle.dump(inputTree, fw)
    fw.close()

#加载决策树
#返回一个字典
def loadTree(filename):
    fr = open(filename, 'rb')
    return pickle.load(fr)