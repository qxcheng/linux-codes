#!/usr/bin/python
# -*- coding: utf-8 -*-

import tree
import treePlot

#创建数据集
def creatDataset():
    fr = open('lenses.txt')
    dataSet = [line.strip().split('\t') for line in fr.readlines()]
    labels = ['age', 'prescript', 'astigmatic', 'tearRate']
    #dataSet = [[1,1,'yes'],[1,1,'yes'],[1,0,'no'],[0,1,'no'],[0,1,'no']]
    #labels = ['no surfacing','flippers']
    return dataSet, labels


def main():
    dataSet, labels = creatDataset()
    myTree = tree.createTree(dataSet,labels)
    classLabel = tree.classify(myTree, ['age', 'prescript', 'astigmatic', 'tearRate'] ,['young','hyper','yes','normal'])
    print(classLabel)

    filename = 'lenses-tree.txt'
    tree.storeTree(myTree, filename)

    tree2 = tree.loadTree(filename)
    print(tree2)

    treePlot.createPlot(tree2)

if __name__ == '__main__':
    main()