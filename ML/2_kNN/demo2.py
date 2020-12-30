import numpy as np
import kNN
import os

# 文本图像转换为一维数组向量
def img2vector(filename):
    returnVect = np.zeros((1, 1024))
    fr = open(filename)
    for i in range(32):
        lineStr = fr.readline()
        for j in range(32):
            returnVect[0, 32 * i + j] = int(lineStr[j])
    return returnVect


# 手写数字识别系统预测函数
def handwritingClassTest():
    hwLabels = []
    trainingFileList = os.listdir("trainingDigits")
    m = len(trainingFileList)
    trainingMat = np.zeros((m, 1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        hwLabels.append(classNumStr)
        trainingMat[i, :] = img2vector("trainingDigits//%s" % fileNameStr)
    testFileList = os.listdir("testDigits")
    errorCount = 0.0
    mTest = len(testFileList)
    for i in range(mTest):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        vectorUnderTest = img2vector("testDigits//%s" % fileNameStr)
        classifierResult = kNN.classify0(vectorUnderTest, trainingMat, hwLabels, 3)
        if (classifierResult != classNumStr):
            errorCount += 1.0
        print("分类器预测的手写数字为：%d, 实际数字为：%d" % (classifierResult, classNumStr))
    print("\n预测失败的总数为：%f" % (errorCount))
    print("\n预测的失败率为：%.2f" % (errorCount / float(mTest) * 100) + "%")


def main():
    handwritingClassTest()


if __name__ == "__main__":
    main()