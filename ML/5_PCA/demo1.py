import numpy as np
import pca
import matplotlib.pyplot as plt

def loadDataSet(fileName, delim='\t'):
    fr = open(fileName)
    stringArr = [line.strip().split(delim) for line in fr.readlines()]
    datArr = [list(map(float,line)) for line in stringArr]
    return np.mat(datArr)

def main():
    dataMat = loadDataSet('testSet.txt')
    lowDMat, reconMat = pca.pca(dataMat, 1)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(dataMat[:,0].flatten().A[0], dataMat[:,1].flatten().A[0], marker='^', s=90)
    ax.scatter(reconMat[:,0].flatten().A[0], dataMat[:,1].flatten().A[0], marker='o', s=50, c='red')
    plt.show()


if __name__ == '__main__':
    main()