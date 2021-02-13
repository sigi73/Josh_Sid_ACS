# Matrix-Matrix Multiplication with SIMD Instructions



## Implementation 

### Matrices
2D matrices of size NxM are represented as 1 dimensional C arrays of length N*M. The arrays are stored in row major form, meaning access to row n, column m is at the (n*M+m) offset from the base. This means that matrix elements in the same row are contiguous in memory, but elements in the same column are not contiguous in memory. Matrices are passed to all functions with a height and width parameter.

### Helper Functions
Several helper functions were created for working with matrices, found in `helper_functions.c` and `helper_functions.h`.  Each function has a short and floating point version. If this project were expanded into a full matrix multiplication library, duplication and  boilerplate code could be reduced by macro generation. Each function is listed below for the short variables, but note a version for float exists as well.

 - `float *create_float_mat(int rows, int cols)`
	 - Creates an floating point matrix of the desired shape initialized to 0.
 - `float *create_random_float_mat(int rows, int cols)`
	 - Creates an floating point matrix of the desired shape initialized to random numbers.
 - `void print_float_mat(float *mat, int rows, int cols)`
	 - Prints the matrix to the console
 - `void clear_float_mat(float *mat, int rows, int cols)`
	 - Sets all matrix elements to 0

### Matrix Multiply Functions
The matrix multiplication functions used are implemented in `matrix_multiply.c` and `matrix_multiply.h`. There are three implementations for each variable type: naive, block (cache-optimized), and simd. The implementation of each is described below.

- Naive
	- TODO
- Block (cache optimized)
	- TODO
- SIMD
	- TODO

### Test files

3 test files were created for testing correctness of matrix multiplication, and evaluating running time.

- `test_float_multiply.c`is used to test the floating point matrix multiplication. Several arrays are multiplied together and the results are compared to an expected output. Expected output was created using the `test_multiply.py` script.
- `test_short_multiply.c`is used to test the short matrix multiplication. Several arrays are multiplied together and the results are compared to an expected output. Expected output was created using the `test_multiply.py` script.
- `custom_matrix.c` is used to prompt the user to enter data type, matrix sizes, and multiplication implementation then reports the time taken for the method.

## Compiling the Code
TODO

## Test Results

| Matrix Sizes  | Int Naive Time(s)	| Int Block Time(s) |	Int SIMD Time(s) |	Float Naive Time(s) |	Float Block Time(s) |	Float SIMD Time(s) |	 
| :---          |  :----:   |  :----:   |  :----:  |   :----:   |   :----:   |   :----:   | 
| 10 x 10	      | 0.000003	| 0.000004	|0.000002	 | 0.000001	  | 0.000005	 | 0.000002   |
| 100 x 100	    | 0.000084	| 0.000228	|0.000107	 | 0.000115	  | 0.000251	 | 0.000141   |
| 500 x 500	    | 0.007163	| 0.013225	|0.008665	 | 0.014144	  | 0.011825	 | 0.015499   |
| 1000 x 1000   |	0.051656	| 0.056296	|0.048288	 | 0.086114	  | 0.087645	 | 0.087941   |
| 2500 x 2500   |	1.154395	| 0.858004	|1.149132	 | 2.469813	  | 1.820505	 | 2.411928   |
| 5000 x 5000   |	10.343661	| 7.515386	|10.298554 | 22.927728  | 14.501678	 | 22.057715  |
| 10000 x 10000 |	92.056563	| 86.815954	|88.892216 | 180.458256 | 175.796184 | 161.798321 |

| Matrix Sizes  | Block Sizes used	| 
| :---          |  :----:   | 
| 10 x 10	      | 2 |
| 100 x 100	    | 16	| 
| 500 x 500	    | 64	|
| 1000 x 1000   |	128	| 
| 2500 x 2500   |	256	| 
| 5000 x 5000   |	512	| 
| 10000 x 10000 | 2048 | 
<img src="https://media.discordapp.net/attachments/366025595257225229/810259659239718920/unknown.png" width="500">

<img src="https://media.discordapp.net/attachments/366025595257225229/810259773479714816/unknown.png" width="500">
