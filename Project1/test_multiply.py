import numpy as np

matrix1 = np.ones((16, 16))
matrix2 = 2 * np.ones((16, 16))
print(np.matmul(matrix1,matrix2))

matrix1 = np.ones((17, 17))
matrix2 = 2 * np.ones((17, 17))
print(np.matmul(matrix1,matrix2))

matrix1 = np.ones((30, 30))
matrix2 = 2 * np.ones((30, 30))
print(np.matmul(matrix1,matrix2))

matrix1 = 1.1*np.ones((8, 8))
matrix2 = 2.5*np.ones((8, 8))
print(np.matmul(matrix1, matrix2))

matrix1 = 1.25*np.arange(8*8).reshape((8,8))
matrix2 = 0.5*np.arange(8*8).reshape((8,8))
print(np.matmul(matrix1, matrix2))

