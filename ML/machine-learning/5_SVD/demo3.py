'''
图像压缩
'''

import numpy as np
from numpy import linalg as la

def printMat(inMat, thresh=0.8):
    for i in range(32):
        a = ''
        for k in range(32):
            if float(inMat[i,k]) > thresh:
                a += '1 '
            else: 
                a += '0 '
        print(a+'\n')

def imgCompress(numSV=3, thresh=0.8):
    myl = []
    for line in open('0_5.txt').readlines():
        newRow = []
        for i in range(32):
            newRow.append(int(line[i]))
        myl.append(newRow)
    myMat = np.mat(myl)
    print("****original matrix******")
    printMat(myMat, thresh)
    U,Sigma,VT = la.svd(myMat)
    SigRecon = np.mat(np.zeros((numSV, numSV)))
    for k in range(numSV):#construct diagonal matrix from vector
        SigRecon[k,k] = Sigma[k]
    reconMat = U[:,:numSV]*SigRecon*VT[:numSV,:]
    print("****reconstructed matrix using %d singular values******" % numSV)
    printMat(reconMat, thresh)

imgCompress()