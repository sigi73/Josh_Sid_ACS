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
