import numpy as np
import adaboost 

def loadDataSet(fileName):      #general function to parse tab -delimited floats
    numFeat = len(open(fileName).readline().split('\t')) #get number of fields 
    dataMat = []; labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr =[]
        curLine = line.strip().split('\t')
        for i in range(numFeat-1):
            lineArr.append(float(curLine[i]))
        dataMat.append(lineArr)
        labelMat.append(float(curLine[-1]))
    return dataMat,labelMat

def main():
    datMat,classLabels = loadDataSet('horseColicTraining2.txt')
    classifierArr = adaboost.adaBoostTrainDS(datMat, classLabels, 30)

    testArr,testLabelArr = loadDataSet('horseColicTest2.txt')
    prediction = adaboost.adaClassify(testArr,classifierArr)
    errArr = np.mat(np.ones((67,1)))
    errRate = errArr[prediction != np.mat(testLabelArr).T].sum() / 67

    print("ERROR RATE: ", errRate)

if __name__ == '__main__':
    main()