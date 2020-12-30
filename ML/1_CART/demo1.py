import numpy as np
import regTrees

#加载数据
def loadDataSet(fileName):      
    dataMat = []                #assume last column is target value
    fr = open(fileName)
    for line in fr.readlines():
        curLine = line.strip().split('\t')
        fltLine = list(map(float,curLine))
        dataMat.append(fltLine)
    return dataMat

'''
#划分函数测试
testMat = np.mat(np.eye(4))
mat0, mat1 = regTrees.binSplitDataSet(testMat,1,0.5)
print(mat0)
print(mat1)
'''

'''
#树回归测试
#myDat = np.mat(loadDataSet('ex00.txt')) #简单数据集
myDat = np.mat(loadDataSet('ex0.txt'))   #分段数据集
myTree  = regTrees.createTree(myDat)
print(myTree)
'''

#模型树测试
myDat = np.mat(loadDataSet('exp2.txt'))   
myTree  = regTrees.createTree(myDat,regTrees.modelLeaf,regTrees.modelErr,(1,10))
print(myTree)
