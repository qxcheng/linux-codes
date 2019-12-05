import numpy as np
import matplotlib.pyplot as plt
import adaboost 

def loadSimpData():
    datMat = np.matrix([[ 1. ,  2.1],
        [ 2. ,  1.1],
        [ 1.3,  1. ],
        [ 1. ,  1. ],
        [ 2. ,  1. ]])
    classLabels = [1.0, 1.0, -1.0, -1.0, 1.0]
    return datMat,classLabels

def main():
    datMat,classLabels = loadSimpData()
    classifierArr = adaboost.adaBoostTrainDS(datMat, classLabels, 30)

    result = adaboost.adaClassify([[3,4],[1,0]],classifierArr)
    print("RESULT:\n",result)

if __name__ == '__main__':
    main()