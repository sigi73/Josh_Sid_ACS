# Matrix-Matrix Multiplication with SIMD Instructions



## Implementation 
### Note about compile flags:
The `-O2` flag was used for compilation instead of `-O3` flag as `-O3` allows the compiler to vectorize the code meaning the naive implementation uses SIMD. Reducing the level of optimization avoids this.

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
The matrix multiplication functions used are implemented in `matrix_multiply.c` and `matrix_multiply.h`. There are three implementations for each variable type: naive, block (cache-optimized), and simd. The implementation of each is described below. Note, the multiplication functions require that the result matrix is all zeros. This is not done in the multiplication function for timing reasons. 

- Naive
	- The naive implementation makes use of 3 nested for loops. The i and j indices are used to index into the destination array. The k index is used to index across the columns of the first matrix and the rows of the second matrix. The j index is kept as the innermost loop as a minor optimization since it refers to adjacent memory locations in the destination array.
- Block (cache optimized)
	- The block mode performs cache optimization by dividing up the multiplication into blocks. This is done by using an additional two nested for loops. The outer two for loops iterate across j and k, but step by a provided "block_size". The next for loop iterates across i normally. The inner two for loops iterate across the same directions as j and k, but only within the current block. This helps with cache optimization by taking advantage of locality, and using data near each other before progressing to new data.
- SIMD
	- The SIMD mode makes use of intrinsics to explicitly used vectorized instructions. Matrix multiplication can be thought of dot products, but since the dot product requires using the column vector of the second matrix, which is not contiguous in memory, and therefore cannot be loaded directly. Instead, the multiplication was accelerated by making use of SIMD to multiply a scalar by a vector. A scalar is loaded from the first matrix into all elements of a 256 bit SIMD vector. Several values (16 shorts or 8 floats) are loaded into another 256 bit SIMD vector. They are multiplied then added to the correct location in the destination matrix. 

### Test files

3 test files were created for testing correctness of matrix multiplication, and evaluating running time.

- `test_float_multiply.c`is used to test the floating point matrix multiplication. Several arrays are multiplied together and the results are compared to an expected output. Expected output was created using the `test_multiply.py` script.
- `test_short_multiply.c`is used to test the short matrix multiplication. Several arrays are multiplied together and the results are compared to an expected output. Expected output was created using the `test_multiply.py` script.
- `custom_matrix.c` is used to prompt the user to enter data type, matrix sizes, and multiplication implementation then reports the time taken for the method.

## Compiling the Code

### cmake on Linux
1. Create a folder named `build` in the same directory as the CMakeLists.txt file
2. Change into the folder (e.g. `$ cd build`)
3. Run cmake to compile in release mode: `$ cmake -DCMAKE_BUILD_TYPE=Release ..`
4. Build by running `$ make`
	1. Can run `make` in parallel with the `-j` flag, eg `$ make -j8`
5. Run the executables created

### cmake on Windows

### gcc on Linux (if cmake is not installed)
1. Change to the `Project 1` directory
2. Test short matrix multiplication: `gcc -O2 -march=native -lm test_short_multiply.c helper_functions.c matrix_multiply.c -o test_short`
3. Test float matrix multiplication: `gcc -O2 -march=native -lm test_float_multiply.c helper_functions.c matrix_multiply.c -o test_float`
4. Interactive test code: `gcc -O2 -march=native -lm custom_matrix.c helper_functions.c matrix_multiply.c -o interactive_matrix`

## Test Results


The following data represents the time for completion of each of the desired data types and the multiplication method used. These tests were ran on an i7-8700k overclocked to 4.5GHz. Every multiplication for this test was done between 2 equally sized matrices. For the block multiplication, different powers of 2 were used and are noted in a separate table. Any other sized block may have different effects on the running time. All of this data is approximate and results may very slightly based on other applications running at the same time as execution. 

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

<img src="https://media.discordapp.net/attachments/366025595257225229/810259659239718920/unknown.png" width="800">

<img src="https://media.discordapp.net/attachments/366025595257225229/810259773479714816/unknown.png" width="800">

## Analysis and Conclusion

From the results of these tests, the difference of running time of all of these different multiplication for both 2-byte fixed-point and 4-byte floating point are all negligible for most situations until the multiplication between two 2500 x 2500 matrices. At this point, for the 2-byte fixed-point matrices, the naive and the SIMD multiplications execute at around the same speed until they diverge at the 5000 x 5000 matrix multiplication. Here the SIMD multiplication is trending to be faster than the naive but is still slower than the block multiplication for the current scale. For the 4-byte floating-point multiplications, the overall trend is the same as the 2-byte fixed-point multiplications. The one major difference is that the SIMD multiplication for the 4-byte floating-point matrices is faster than the block multiplication for the 10000 x 10000 matrix multiplication. Looking at this trend, the SIMD multiplication for the 2-byte fixed-point may become faster than the block multiplication for larger matrices. 

In conclusion, for very large matrix multiplications, the use of SIMD instructions may greatly reduce the overall run time and be the most optimal solution for that test case. For smaller test cases however, the implementation of the matrix multiplication does not matter as much unless when processes that go down to the microsecond are of concern.

