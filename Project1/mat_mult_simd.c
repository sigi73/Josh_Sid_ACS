#include <stdio.h>

#include "helper_functions.h"
#include "matrix_multiply.h"

void main()
{
    // RIGHT NOW ONLY WORKS WITH MULTIPLES OF 16
    int SIZE = 32;
    int16_t *matrix1 = create_short_mat(SIZE, SIZE);
    int16_t *matrix2 = create_short_mat(SIZE, SIZE);
    int16_t *result = create_short_mat(SIZE, SIZE);

    for (int i = 0; i < SIZE*SIZE; i++)
    {
        matrix1[i] = 1;
        matrix2[i] = 2;
    }

    printf("Matrix 1\n");
    print_short_mat(matrix1, SIZE, SIZE);
    printf("Matrix 2\n");
    print_short_mat(matrix2, SIZE, SIZE);

    multiply_short_simd(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, result);
    print_short_mat(result, SIZE, SIZE);


}