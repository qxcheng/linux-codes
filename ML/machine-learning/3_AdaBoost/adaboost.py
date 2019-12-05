import numpy as np
import matplotlib.pyplot as plt

#根据阈值分类数据集
def stumpClassify(dataMatrix,dimen,threshVal,threshIneq):
    retArray = np.ones((np.shape(dataMatrix)[0],1))  #为m
    if threshIneq == 'lt':
        retArray[dataMatrix[:,dimen] <= threshVal] = -1.0
    else:
        retArray[dataMatrix[:,dimen] > threshVal] = -1.0
    return retArray

#构建单层决策树
def buildStump(dataArr,classLabels,D):
    dataMatrix = np.mat(dataArr)      #mxn样本
    labelMat = np.mat(classLabels).T  #mx1标签
    m,n = np.shape(dataMatrix)        
    numSteps = 10.0;  #步数
    bestStump = {}    #存储单层决策树
    bestClasEst = np.mat(np.zeros((m,1)))
    minError = np.inf  
    #遍历每个特征
    for i in range(n): 
        rangeMin = dataMatrix[:,i].min() #第i列最小值
        rangeMax = dataMatrix[:,i].max() #第i列最大值
        stepSize = (rangeMax-rangeMin)/numSteps #步长
        #阈值遍历第i列取值范围
        for j in range(-1,int(numSteps)+1):            
            for inequal in ['lt', 'gt']:
                threshVal = (rangeMin + float(j) * stepSize) #阈值
                predictedVals = stumpClassify(dataMatrix,i,threshVal,inequal) #根据阈值分类，返回mx1标签
                errArr = np.mat(np.ones((m,1)))       #错误率
                errArr[predictedVals == labelMat] = 0 #==产生布尔值数组，为真时相应位置赋值0
                weightedError = D.T*errArr            #根据权重D计算总错误率
                print("split: dim %d, thresh %.2f, thresh ineqal: %s, the weighted error is %.3f" % (i, threshVal, inequal, weightedError))
                if weightedError < minError:
                    minError = weightedError
                    bestClasEst = predictedVals.copy() #mx1标签的复制
                    bestStump['dim'] = i
                    bestStump['thresh'] = threshVal
                    bestStump['ineq'] = inequal
    return bestStump,minError,bestClasEst

#adaboost训练
def adaBoostTrainDS(dataArr,classLabels,numIt=40):
    weakClassArr = []                     #存储弱分类器字典
    m = np.shape(dataArr)[0]
    D = np.mat(np.ones((m,1))/m)          #mx1,每个样本的初始权重为1/m
    aggClassEst = np.mat(np.zeros((m,1))) #mx1,
    #迭代numIt次
    for i in range(numIt):
        bestStump,error,classEst = buildStump(dataArr,classLabels,D) #最好的弱分类器
        print("D:",D.T)
        alpha = float(0.5*np.log((1.0-error)/max(error,1e-16))) #计算弱分类器的权重, max防止分母为0
        bestStump['alpha'] = alpha                              
        weakClassArr.append(bestStump)  #存储弱分类器
        print("classEst: ",classEst.T)
        expon = np.multiply(-1*alpha*np.mat(classLabels).T,classEst) #更新样本权重D
        D = np.multiply(D,np.exp(expon))                              
        D = D/D.sum()
        aggClassEst += alpha*classEst        #分类器权重float x 预测标签mx1，累加每个分类器，越来越精准
        print("aggClassEst: ",aggClassEst.T)
        aggErrors = np.multiply(np.sign(aggClassEst) != np.mat(classLabels).T,np.ones((m,1)))
        errorRate = aggErrors.sum()/m   #总错误率，每次重新算
        print("total error: ",errorRate)
        if errorRate == 0.0: 
            break
    return weakClassArr #返回弱分类器字典的列表

#classify
def adaClassify(datToClass,classifierArr):
    dataMatrix = np.mat(datToClass)
    m = np.shape(dataMatrix)[0]
    aggClassEst = np.mat(np.zeros((m,1))) #存储分类标签
    for i in range(len(classifierArr)):
        classEst = stumpClassify(dataMatrix,classifierArr[i]['dim'],\
                                 classifierArr[i]['thresh'],\
                                 classifierArr[i]['ineq'])
        aggClassEst += classifierArr[i]['alpha']*classEst
        print(aggClassEst)
    return np.sign(aggClassEst)

#plot
def plotROC(predStrengths, classLabels):
    cur = (1.0,1.0) #cursor
    ySum = 0.0 #variable to calculate AUC
    numPosClas = sum(np.array(classLabels)==1.0)
    yStep = 1/float(numPosClas); xStep = 1/float(len(classLabels)-numPosClas)
    sortedIndicies = predStrengths.argsort()#get sorted index, it's reverse
    fig = plt.figure()
    fig.clf()
    ax = plt.subplot(111)
    #loop through all the values, drawing a line segment at each point
    for index in sortedIndicies.tolist()[0]:
        if classLabels[index] == 1.0:
            delX = 0; delY = yStep;
        else:
            delX = xStep; delY = 0;
            ySum += cur[1]
        #draw line from cur to (cur[0]-delX,cur[1]-delY)
        ax.plot([cur[0],cur[0]-delX],[cur[1],cur[1]-delY], c='b')
        cur = (cur[0]-delX,cur[1]-delY)
    ax.plot([0,1],[0,1],'b--')
    plt.xlabel('False positive rate'); plt.ylabel('True positive rate')
    plt.title('ROC curve for AdaBoost horse colic detection system')
    ax.axis([0,1,0,1])
    plt.show()
    print("the Area Under the Curve is: ",ySum*xStep)