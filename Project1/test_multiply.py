import numpy as np

np.set_printoptions(precision=10)

# Short test case 1
matrix1 = np.ones((17, 17))
matrix2 = 2 * np.ones((17, 17))
print(np.matmul(matrix1, matrix2))

# Short test case 2
matrix1 = np.arange(17*17).reshape((17, 17))
matrix2 = np.tile(np.arange(17), (17, 1))
mult = np.matmul(matrix1, matrix2)
one_dimensional = np.array2string(mult.reshape(17*17), separator=',', max_line_width=1000000)
print(matrix1)
print(matrix2)
print(mult)
print(one_dimensional)

# Float test case 1
matrix1 = 1.1*np.ones((17, 17))
matrix2 = 2.5*np.ones((17, 17))
print(np.matmul(matrix1, matrix2))

# Float test case 2
matrix1 = np.arange(17*17).reshape((17, 17))*2.8
matrix2 = np.tile(np.arange(17), (17, 1))*3.7
mult = np.matmul(matrix1, matrix2)
one_dimensional = np.array2string(mult.reshape(17*17), separator=',', max_line_width=1000000)
print(matrix1)
print(matrix2)
print(mult)
print(one_dimensional)

