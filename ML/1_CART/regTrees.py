import numpy as np

#根据特征阈值分割矩阵
def binSplitDataSet(dataSet, feature, value):
    mat0 = dataSet[np.nonzero(dataSet[:,feature] > value)[0],:]
    mat1 = dataSet[np.nonzero(dataSet[:,feature] <= value)[0],:]
    return mat0,mat1

#建立叶节点
#returns the value used for each leaf
def regLeaf(dataSet):
    return np.mean(dataSet[:,-1])

#总误差计算
def regErr(dataSet):
    return np.var(dataSet[:,-1]) * np.shape(dataSet)[0]

#dataSet：数据集
#leafType：建立叶节点的函数
#errType：误差计算函数
#ops：其他参数
def chooseBestSplit(dataSet, leafType=regLeaf, errType=regErr, ops=(1,4)):
    tolS = ops[0] #容许的误差下降值
    tolN = ops[1] #切分的最少样本数
    #exit cond 1
    #if all the target variables are the same value: quit and return value
    if len(set(dataSet[:,-1].T.tolist()[0])) == 1: 
        return None, leafType(dataSet)
    m,n = np.shape(dataSet)
    #the choice of the best feature is driven by Reduction in RSS error from mean
    S = errType(dataSet)
    bestS = np.inf
    bestIndex = 0
    bestValue = 0
    for featIndex in range(n-1):
        for splitVal in set(dataSet[:,featIndex].T.A.tolist()[0]):
            mat0, mat1 = binSplitDataSet(dataSet, featIndex, splitVal)
            #样本数 <tolN 时不允许该切分
            if (np.shape(mat0)[0] < tolN) or (np.shape(mat1)[0] < tolN): 
                continue
            newS = errType(mat0) + errType(mat1)
            if newS < bestS: 
                bestIndex = featIndex
                bestValue = splitVal
                bestS = newS
    #exit cond 2
    #if the decrease (S-bestS) is less than a threshold don't do the split
    #即切分后提升效果不大
    if (S - bestS) < tolS: 
        return None, leafType(dataSet) 
    mat0, mat1 = binSplitDataSet(dataSet, bestIndex, bestValue)
    #exit cond 3
    #最好的切分方式下，样本数 <tolN （只能是0，0？）
    if (np.shape(mat0)[0] < tolN) or (np.shape(mat1)[0] < tolN):  
        return None, leafType(dataSet)
    return bestIndex,bestValue
                              
#assume dataSet is NumPy Mat so we can array filtering
def createTree(dataSet, leafType=regLeaf, errType=regErr, ops=(1,4)):
    feat, val = chooseBestSplit(dataSet, leafType, errType, ops)
    if feat == None: 
        return val #if the splitting hit a stop condition return val
    retTree = {}
    retTree['spInd'] = feat
    retTree['spVal'] = val
    lSet, rSet = binSplitDataSet(dataSet, feat, val)
    retTree['left'] = createTree(lSet, leafType, errType, ops)
    retTree['right'] = createTree(rSet, leafType, errType, ops)
    return retTree  


#以下三个函数为剪枝相关
#判断对象是否是字典
def isTree(obj):
    return (type(obj).__name__=='dict')

def getMean(tree):
    if isTree(tree['right']): 
        tree['right'] = getMean(tree['right'])
    if isTree(tree['left']): 
        tree['left'] = getMean(tree['left'])
    return (tree['left']+tree['right'])/2.0
    
#tree：构造的回归树
#testData：测试数据集
def prune(tree, testData):
    #if we have no test data collapse the tree
    if np.shape(testData)[0] == 0: 
        return getMean(tree) 
    #if the branches are not trees try to prune them
    if (isTree(tree['right']) or isTree(tree['left'])):
        lSet, rSet = binSplitDataSet(testData, tree['spInd'], tree['spVal'])
    if isTree(tree['left']): 
        tree['left'] = prune(tree['left'], lSet)
    if isTree(tree['right']): 
        tree['right'] =  prune(tree['right'], rSet)
    #if they are now both leafs, see if we can merge them
    if not isTree(tree['left']) and not isTree(tree['right']):
        lSet, rSet = binSplitDataSet(testData, tree['spInd'], tree['spVal'])
        errorNoMerge = sum(np.power(lSet[:,-1] - tree['left'],2)) +\
                       sum(np.power(rSet[:,-1] - tree['right'],2))
        treeMean = (tree['left']+tree['right'])/2.0
        errorMerge = sum(np.power(testData[:,-1] - treeMean, 2))
        if errorMerge < errorNoMerge: 
            print ("merging")
            return treeMean
        else: 
            return tree
    else: 
        return tree


#以下三个函数为模型树
#helper function used in two places
def linearSolve(dataSet):  
    m,n = np.shape(dataSet)
    X = np.mat(np.ones((m,n)))
    Y = np.mat(np.ones((m,1)))#create a copy of data with 1 in 0th postion
    X[:,1:n] = dataSet[:,0:n-1]
    Y = dataSet[:,-1]#and strip out Y
    xTx = X.T*X
    if np.linalg.det(xTx) == 0.0:
        raise NameError('This matrix is singular, cannot do inverse,\n\
        try increasing the second value of ops')
    ws = xTx.I * (X.T * Y)
    return ws,X,Y

#create linear model and return coeficients
def modelLeaf(dataSet):
    ws,X,Y = linearSolve(dataSet)
    return ws

def modelErr(dataSet):
    ws,X,Y = linearSolve(dataSet)
    yHat = X * ws
    return sum(np.power(Y - yHat,2))   


#以下函数为预测代码
def regTreeEval(model, inDat):
    return float(model)

def modelTreeEval(model, inDat):
    n = np.shape(inDat)[1]
    X = np.mat(np.ones((1,n+1)))
    X[:,1:n+1]=inDat
    return float(X*model)

def treeForeCast(tree, inData, modelEval=regTreeEval):
    if not isTree(tree): 
        return modelEval(tree, inData)
    if inData[tree['spInd']] > tree['spVal']:
        if isTree(tree['left']): 
            return treeForeCast(tree['left'], inData, modelEval)
        else: 
            return modelEval(tree['left'], inData)
    else:
        if isTree(tree['right']): 
            return treeForeCast(tree['right'], inData, modelEval)
        else: 
            return modelEval(tree['right'], inData)
        
def createForeCast(tree, testData, modelEval=regTreeEval):
    m=len(testData)
    yHat = np.mat(np.zeros((m,1)))
    for i in range(m):
        yHat[i,0] = treeForeCast(tree, np.mat(testData[i]), modelEval)
    return yHat