import numpy as np
import operator

# k近邻分类器
# inx:待预测特征   1 x n
# dataSet:样本集   m x n
# labels:标签      m x 1
# k: 一般小于20
def classify0(inX, dataSet, lables, k):
    dataSetSize = dataSet.shape[0]                      # 得到样本行数 m
    diffMat = np.tile(inX, (dataSetSize, 1)) - dataSet  # 竖向填充 m 次，横向填充一次，再与样本集相减

    sqDiffMat = diffMat ** 2                            #计算欧式距离
    sqDistances = sqDiffMat.sum(axis=1)
    distances = sqDistances ** 0.5
    soretedDistIndices = distances.argsort()            #从小到大排序得到对应索引

    classCount = {}                                     #统计前 k 个样本的标签个数并排序 取最大值的标签返回
    for i in range(k):
        voteIlabel = lables[soretedDistIndices[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)

    return sortedClassCount[0][0]


