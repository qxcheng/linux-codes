# coding=UTF-8
import numpy as np
import kMeans

def main():
    dataMat = np.mat(kMeans.loadDataSet("testSet2.txt"))
    #centroids, clustAssment = kMeans.kMeans(dataMat,4)
    centroids, clustAssment = kMeans.biKmeans(dataMat, 3)
    print(centroids)
    print(clustAssment)

if __name__ == '__main__':
    main()