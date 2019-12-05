import numpy as np
from numpy import linalg

def loadExData():
    return[[0, 0, 0, 2, 2],
           [0, 0, 0, 3, 3],
           [0, 0, 0, 1, 1],
           [1, 1, 1, 0, 0],
           [2, 2, 2, 0, 0],
           [5, 5, 5, 0, 0],
           [1, 1, 1, 0, 0]]

Data = loadExData()
U, Sigma, VT = linalg.svd(Data)       #分解
print(Sigma)
Sig3 = np.mat([[Sigma[0],0,0], [0,Sigma[1],0], [0,0,Sigma[2]]])
rarData = U[:, :3] * Sig3 * VT[:3, :] #复原
print(rarData)