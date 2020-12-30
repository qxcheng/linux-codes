# coding=UTF-8
import numpy as np

#加载数据
def loadDataSet(fileName):      #general function to parse tab -delimited floats
    dataMat = []                #assume last column is target value
    fr = open(fileName)
    for line in fr.readlines():
        curLine = line.strip().split('\t')
        fltLine = list(map(float,curLine)) #map all elements to float()
        dataMat.append(fltLine)
    return dataMat

# 距离计算函数
def distEclud(vecA, vecB):
    return np.sqrt(sum(np.power(vecA - vecB, 2).tolist()[0])) #la.norm(vecA-vecB)

#随机初始化k个聚类中心
def randCent(dataSet, k):
    n = np.shape(dataSet)[1]
    centroids = np.mat(np.zeros((k,n)))
    for j in range(n):       
        minJ = min(dataSet[:,j]) 
        maxJ = max(dataSet[:,j])
        rangeJ = float(maxJ - minJ)
        centroids[:,j] = np.mat(minJ + rangeJ * np.random.rand(k,1))
    return centroids
    
#k均值聚类
def kMeans(dataSet, k, distMeas=distEclud, createCent=randCent):
    m = np.shape(dataSet)[0]
    #create mat to assign data points to a centroid, also holds SE of each point
    clusterAssment = np.mat(np.zeros((m,2)))                                
    centroids = createCent(dataSet, k)
    clusterChanged = True
    while clusterChanged:
        clusterChanged = False
        #for each data point assign it to the closest centroid
        for i in range(m):
            minDist = np.inf
            minIndex = -1
            for j in range(k):
                distJI = distMeas(centroids[j,:],dataSet[i,:])               
                if distJI < minDist:
                    minDist = distJI
                    minIndex = j
            if clusterAssment[i,0] != minIndex: 
                clusterChanged = True
            clusterAssment[i,:] = minIndex,minDist**2
        print(centroids)
        #recalculate centroids
        for cent in range(k):
            #get all the point in this cluster
            ptsInClust = dataSet[np.nonzero(clusterAssment[:,0].A==cent)[0]]
            #assign centroid to mean
            centroids[cent,:] = np.mean(ptsInClust, axis=0)  
    return centroids, clusterAssment

#二分k均值聚类
def biKmeans(dataSet, k, distMeas=distEclud):
    m = np.shape(dataSet)[0]                          # 样本总数
    clusterAssment = np.mat(np.zeros((m,2)))          # 第一列保存标签，第二列保存该样本到其聚类中心的距离
    centroid0 = np.mean(dataSet, axis=0).tolist()[0]  # 取所有样本均值为第一个聚类中心
    centList =[centroid0]                             # 保存聚类中心的列表
    # 计算距离
    for j in range(m):
        clusterAssment[j,1] = distMeas(np.mat(centroid0), dataSet[j,:])**2
    # 当簇的个数小于k时
    while (len(centList) < k): 
        lowestSSE = np.inf  # 每次循环初始化最小总误差
        # 遍历每个簇
        for i in range(len(centList)):
            # 得到属于第i个簇的样本点
            ptsInCurrCluster = dataSet[np.nonzero(clusterAssment[:,0].A==i)[0],:]
            # 对第i个簇进行二分均值聚类，得到2xn的两个聚类中心 和 mx2的01标签、距离矩阵
            centroidMat, splitClustAss = kMeans(ptsInCurrCluster, 2, distMeas)
            # 计算总误差
            sseSplit = sum(splitClustAss[:,1])
            sseNotSplit = sum(clusterAssment[np.nonzero(clusterAssment[:,0].A!=i)[0],1])
            print("sseSplit, and notSplit: ",sseSplit,sseNotSplit)
            if (sseSplit + sseNotSplit) < lowestSSE:
                bestCentToSplit = i                   # 最好的簇
                bestNewCents = centroidMat            # 两个聚类中心
                bestClustAss = splitClustAss.copy()   # mx2标签距离矩阵
                lowestSSE = sseSplit + sseNotSplit    # 最小总误差
        #改变标签，1标签添加为末尾标签，0标签改为原始第i标签
        bestClustAss[np.nonzero(bestClustAss[:,0].A == 1)[0],0] = len(centList) 
        bestClustAss[np.nonzero(bestClustAss[:,0].A == 0)[0],0] = bestCentToSplit
        print('the bestCentToSplit is: ',bestCentToSplit)
        print('the len of bestClustAss is: ', len(bestClustAss))
        #更新聚类中心，第0中心改为原始第i中心，第1中心添加到末尾
        centList[bestCentToSplit] = bestNewCents[0,:].tolist()[0]
        centList.append(bestNewCents[1,:].tolist()[0])
        #更新执行二分聚类的簇的标签结果矩阵到原始标签距离矩阵 
        clusterAssment[np.nonzero(clusterAssment[:,0].A == bestCentToSplit)[0],:]= bestClustAss
    return np.mat(centList), clusterAssment


