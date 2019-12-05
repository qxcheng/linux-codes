import numpy as np
import matplotlib.pyplot as plt
import regression 


def loadDataSet(fileName):      #general function to parse tab -delimited floats
    numFeat = len(open(fileName).readline().split('\t')) - 1 #get number of fields 
    dataMat = []; labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr =[]
        curLine = line.strip().split('\t')
        for i in range(numFeat):
            lineArr.append(float(curLine[i]))
        dataMat.append(lineArr)
        labelMat.append(float(curLine[-1]))
    return dataMat,labelMat

#标准线性回归测试
def standRegresTest():
    xArr, yArr = loadDataSet('ex0.txt')
    ws = regression.standRegres(xArr, yArr)
    xMat = np.mat(xArr)
    yMat = np.mat(yArr)
    yHat = xMat * ws

    #计算相关系数
    #np.corrcoef(yHat.T, yMat)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xMat[:,1].flatten().A[0], yMat.T[:,0].flatten().A[0])

    xCopy = xMat.copy()
    xCopy.sort(0)
    yHat = xCopy * ws
    ax.plot(xCopy[:,1], yHat)
    plt.show()

#局部加权线性回归测试
def lwlrTest(k=1.0):  
    xArr, yArr = loadDataSet('ex0.txt')
    xMat = np.mat(xArr)
    yMat = np.mat(yArr)
    m = np.shape(xArr)[0]
    yHat = np.zeros(m)
  
    xCopy = np.mat(xArr)
    xCopy.sort(0)
    for i in range(m):
        yHat[i] = regression.lwlr(xCopy[i],xArr,yArr,k)
    
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xMat[:,1].flatten().A[0], yMat.T[:,0].flatten().A[0])

    xCopy = xMat.copy()
    xCopy.sort(0)
    ax.plot(xCopy[:,1], yHat)
    plt.show()

    
#岭回归测试
def ridgeTest():
    xArr, yArr = loadDataSet('abalone.txt')
    xMat = np.mat(xArr) 
    yMat=np.mat(yArr).T
    yMean = np.mean(yMat,0)
    yMat = yMat - yMean        #to eliminate X0 take mean off of Y
    xMeans = np.mean(xMat,0)   #calc mean then subtract it off
    xVar = np.var(xMat,0)      #calc variance of Xi then divide by it
    xMat = (xMat - xMeans)/xVar

    numTestPts = 30
    wMat = np.zeros((numTestPts, np.shape(xMat)[1]))
    for i in range(numTestPts):
        ws = regression.ridgeRegres(xMat,yMat,np.exp(i-10))
        wMat[i,:]=ws.T

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(wMat)
    plt.show()

def stageWiseTest():
    xArr, yArr = loadDataSet('abalone.txt')
    ws = regression.stageWise(xArr,yArr,0.001,5000)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(ws)
    plt.show()


if __name__ == '__main__':
    #standRegresTest()
    
    #lwlrTest(0.01)

    #ridgeTest()

    stageWiseTest()
    

    