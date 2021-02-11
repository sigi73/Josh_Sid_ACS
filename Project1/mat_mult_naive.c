#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <unistd.h>

#include "helper_functions.h"
#include "matrix_multiply.h"

void main()
{
    printf("pid: %ld\n", getpid());
    //int16_t *matrix1 = create_random_short_mat(2, 2);
    //int16_t *matrix2 = create_random_short_mat(2, 2);
    //creating empty matrixes
    int16_t *matrix1 = create_short_mat(3, 2);
    int16_t *matrix2 = create_short_mat(2, 3);
    int16_t *result = create_short_mat(3, 3);
    //populating  martrixes with known values to verify math
    matrix1[0] = 1;
    matrix1[1] = 2;
    matrix1[2] = 3;
    matrix1[3] = 4;
    matrix1[4] = 5;
    matrix1[5] = 6;

    matrix2[0] = 7;
    matrix2[1] = 8;
    matrix2[2] = 9;
    matrix2[3] = 10;
    matrix2[4] = 11;
    matrix2[5] = 12;
    //Printing Matrixes for testing
    printf("Matrix 1\n");
    print_short_mat(matrix1, 3, 2);
    printf("Matrix 2\n");
    print_short_mat(matrix2, 2, 3);
    //mutliplying matrix1 and 2 in traditional way
    multiply_short_naive(matrix1, 3, 2, matrix2, 2, 3, result);
    printf("----\n");
    print_short_mat(result, 3, 3);
    clear_mat(result, 3, 3);
    //multiplying matrix1 and 2 in block meathod to take advantage of localization
    multiply_short_block(matrix1, 3, 2, matrix2, 2, 3, 2, result);
    printf("----\n");
    print_short_mat(result, 3, 3);
    clear_mat(result, 3, 3);

    free(matrix1);
    free(matrix2);
    free(result);

    float *matrix3 = create_random_float_mat(3, 2);
    float *matrix4 = create_random_float_mat(2, 3);
    float *result2 = create_random_float_mat(3, 3);

    printf("Matrix 3\n");
    print_float_mat(matrix3, 3, 2);
    printf("Matrix 4\n");
    print_float_mat(matrix4, 2, 3);

    multiply_float_naive(matrix3, 3, 2, matrix4, 2, 3, result2);
    printf("----\n");
    print_float_mat(result2, 3, 3);
    clear_float_mat(result2, 3, 3);

    multiply_float_block(matrix3, 3, 2, matrix4, 2, 3, 2, result2);
    printf("----\n");
    print_float_mat(result2, 3, 3);
    clear_float_mat(result2, 3, 3);

    free(matrix3);
    free(matrix4);
    free(result2);


    // //prepping timer
    // struct timeval time_start;
	// struct timeval time_end;

    // //Starts The 10k x 10k matrix multiplication (min 1GB open memory reccomended)
    // int SIZE = 10000;
    // printf("Starting %d x %d\n", SIZE, SIZE);
    // matrix1 = create_random_short_mat(SIZE, SIZE);
    // matrix2 = create_random_short_mat(SIZE, SIZE);
    // result = create_short_mat(SIZE, SIZE);
	// gettimeofday(&time_start, NULL);
    // multiply_short_naive(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, result);
    // gettimeofday(&time_end, NULL);

    // long long multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	// printf("%lld\n", multiply_time);

    // //Starts The 10k x 10k matrix multiplication with block meathod (min 1GB open memory reccomended)
    // printf("Starting %d x %d block\n", SIZE, SIZE);
    // //clear_mat(result, SIZE, SIZE);
    // gettimeofday(&time_start, NULL);
    // multiply_short_block(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, 2048, result);
    // gettimeofday(&time_end, NULL);
    // multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	// printf("%lld\n", multiply_time);

    // free(result);
    // free(matrix1);
    // free(matrix2);

    //printf("Finished %d x %d\n", SIZE, SIZE);
}