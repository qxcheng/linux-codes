# coding=utf-8
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import kNN

# 文本文件解析函数
def file2matrix(filename):
    fr = open(filename)
    arrayLines = fr.readlines()
    numberOflines = len(arrayLines)

    returnMat = np.zeros((numberOflines, 3))  # 填充同型零维矩阵
    classLabelVector = []
    index = 0
    for line in arrayLines:                              # 遍历每一行
        line = line.strip()                              # 去两边空格
        listFromLine = line.split('\t')                  # 每一行转换为列表
        returnMat[index, :] = listFromLine[0:3]          # 特征取每行前三列
        classLabelVector.append(int(listFromLine[-1]))   # 标签取最后一列
        index += 1
    return returnMat, classLabelVector


# 归一化数值,使特征值权重相等
def autoNorm(dataSet):
    minValues = dataSet.min(0)                 # 得到每列的最小值，一维数组
    maxValues = dataSet.max(0)                 # 得到每列的最大值，一维数组
    ranges = maxValues - minValues
    normDataSet = np.zeros(np.shape(dataSet))  # 填充同型零矩阵
    m = dataSet.shape[0]
    normDataSet = dataSet - np.tile(minValues, (m, 1))  # 归一化公式：newValue=(oldValue-min)/(max-min)
    normDataSet = normDataSet / np.tile(ranges, (m, 1))
    return normDataSet, ranges, minValues


# 测试代码
def datingClassTest(filename):
    hoRatio = 0.10
    datingDataMat, datingLabels = file2matrix(filename)
    normMat, ranges, minValues = autoNorm(datingDataMat)
    m = normMat.shape[0]
    numTestVecs = int(m * hoRatio)       #测试的样本数
    errorCount = 0.0
    for i in range(numTestVecs):
        classifierResult = kNN.classify0(normMat[i, :], normMat[numTestVecs:m, :], datingLabels[numTestVecs:m], 50)
        print("分类器预测结果为: %d, 实际值为: %d" % (classifierResult, datingLabels[i]))
        if classifierResult != datingLabels[i]:
            errorCount += 1.0
    print('分类器错误率为：%.2f' % (errorCount / float(numTestVecs) * 100) + '%')


# 预测函数
def classifyPerson(filename):
    resultList = ['一点也不', '比较可能', '非常可能']
    percentTats = float(input("打游戏时间所占的比例？"))
    ffMiles = float(input("每年获得的飞行常客里程数？"))
    iceCream = float(input("每周冰激凌消费的公斤数？"))
    inArr = np.array([ffMiles, percentTats, iceCream])
    datingDataMat, datingLabels = file2matrix(filename)
    normMat, ranges, minValues = autoNorm(datingDataMat)
    classifierResult = kNN.classify0((inArr - minValues) / ranges, normMat, datingLabels, 50)
    print("你喜欢这个人的可能性是：", resultList[classifierResult - 1])

 #以第一列和第二列数据画散点图
def dataPlot(filename):
    datingDataMat, datingLabels = file2matrix(filename)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(datingDataMat[:,0], datingDataMat[:,1], 15.0*np.array(datingLabels), 15.0*np.array(datingLabels))
    plt.show()


def main():
    filename = "datingTestSet2.txt"
    dataPlot(filename)
    datingClassTest(filename)
    #classifyPerson(filename)



if __name__ == '__main__':
    main()