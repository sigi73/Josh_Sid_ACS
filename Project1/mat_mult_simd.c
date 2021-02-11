#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "helper_functions.h"
#include "matrix_multiply.h"

void main()
{
    int SIZE = 17;
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


    clear_mat(matrix1, SIZE, SIZE);
    clear_mat(matrix2, SIZE, SIZE);
    clear_mat(result, SIZE, SIZE);
    for (int i = 0; i < SIZE*SIZE; i++)
    {
        matrix1[i] = i;
        matrix2[i] = 0;
    }
    for (int i = 0; i < SIZE; i++)
    {
        matrix2[i*SIZE + i] = 1;
    }

    printf("Matrix 1\n");
    print_short_mat(matrix1, SIZE, SIZE);
    printf("Matrix 2\n");
    print_short_mat(matrix2, SIZE, SIZE);

    multiply_short_simd(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, result);
    print_short_mat(result, SIZE, SIZE);


    free(matrix1);
    free(matrix2);
    free(result);

    struct timeval time_start;
	struct timeval time_end;

    SIZE = 4096;
    printf("Starting %d x %d\n", SIZE, SIZE);
    matrix1 = create_random_short_mat(SIZE, SIZE);
    matrix2 = create_random_short_mat(SIZE, SIZE);
    result = create_short_mat(SIZE, SIZE);
	gettimeofday(&time_start, NULL);
    multiply_short_naive(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, result);
    gettimeofday(&time_end, NULL);

    long long multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	printf("%lld\n", multiply_time);


    printf("Starting %d x %d simd\n", SIZE, SIZE);
    //clear_mat(result, SIZE, SIZE);
    gettimeofday(&time_start, NULL);
    multiply_short_simd(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, result);
    gettimeofday(&time_end, NULL);
    multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	printf("%lld\n", multiply_time);

    free(result);
    free(matrix1);
    free(matrix2);

    printf("Finished %d x %d\n", SIZE, SIZE);
}