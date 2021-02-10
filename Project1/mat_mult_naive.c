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
    int16_t *matrix1 = create_short_mat(3, 2);
    int16_t *matrix2 = create_short_mat(2, 3);
    int16_t *result = create_short_mat(3, 3);

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

    printf("Matrix 1\n");
    print_short_mat(matrix1, 3, 2);
    printf("Matrix 2\n");
    print_short_mat(matrix2, 2, 3);

    multiply_short_naive(matrix1, 3, 2, matrix2, 2, 3, result);
    printf("----\n");
    print_short_mat(result, 3, 3);
    clear_mat(result, 3, 3);

    multiply_short_block(matrix1, 3, 2, matrix2, 2, 3, 2, result);
    printf("----\n");
    print_short_mat(result, 3, 3);
    clear_mat(result, 3, 3);

    free(matrix1);
    free(matrix2);
    free(result);

    struct timeval time_start;
	struct timeval time_end;

    int SIZE = 10000;
    printf("Starting %d x %d\n", SIZE, SIZE);
    matrix1 = create_random_short_mat(SIZE, SIZE);
    matrix2 = create_random_short_mat(SIZE, SIZE);
    result = create_short_mat(SIZE, SIZE);
	gettimeofday(&time_start, NULL);
    multiply_short_naive(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, result);
    gettimeofday(&time_end, NULL);

    long long multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	printf("%lld\n", multiply_time);


    printf("Starting %d x %d block\n", SIZE, SIZE);
    //clear_mat(result, SIZE, SIZE);
    gettimeofday(&time_start, NULL);
    multiply_short_block(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, 2048, result);
    gettimeofday(&time_end, NULL);
    multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	printf("%lld\n", multiply_time);

    free(result);
    free(matrix1);
    free(matrix2);

    printf("Finished %d x %d\n", SIZE, SIZE);
}