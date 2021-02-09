#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <unistd.h>

#include "helper_functions.h"

void multiply(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int16_t **result) 
{
    //check cols1==rows2


/*
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols2; j++)
        {
            for (int k = 0; k < cols1; k++)
            {
                (*result)[i * cols2 + j] += m1[i*cols1 + k] * m2[k*cols2 + j];
                //printf("%d, %d, %d, %d\n", i, j, k, result[i*rows1+j]);
            }
        }
    }
    */
    for (int k = 0; k < cols1; k++)
    {
        for (int j = 0; j < cols2; j++)
        {
            for (int i = 0; i < rows1; i++)
            {
                (*result)[i * cols2 + j] += m1[i*cols1 + k] * m2[k*cols2 + j];
                //printf("%d, %d, %d, %d\n", i, j, k, result[i*rows1+j]);
            }
        }
    }
}

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

    multiply(matrix1, 3, 2, matrix2, 2, 3, &result);

    print_short_mat(result, 3, 3);

    free(matrix1);
    free(matrix2);
    free(result);

    struct timeval time_start;
	struct timeval time_end;

    printf("Starting 1k x 1k\n");
    int SIZE = 1000;
    matrix1 = create_random_short_mat(SIZE, SIZE);
    matrix2 = create_random_short_mat(SIZE, SIZE);
    for (int i = 0; i < SIZE*SIZE; i++)
    {
        matrix1[i] = 1;
        matrix2[i] = 1;
    }
    result = create_short_mat(SIZE, SIZE);
	gettimeofday(&time_start, NULL);
    multiply(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, &result);
    gettimeofday(&time_end, NULL);

    long long multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	printf("%lld,", multiply_time);

    free(matrix1);
    free(matrix2);
    free(result);


    printf("Finished 1k\n");

/*
    printf("Starting 2k x 2k\n");
    SIZE = 2000;
    matrix1 = create_random_short_mat(SIZE, SIZE);
    matrix2 = create_random_short_mat(SIZE, SIZE);
    result = create_short_mat(SIZE, SIZE);
    multiply(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, &result);
    free(matrix1);
    free(matrix2);
    free(result);
    printf("Finished 2k\n");

    printf("Starting 3k x 3k\n");
    SIZE = 3000;
    matrix1 = create_random_short_mat(SIZE, SIZE);
    matrix2 = create_random_short_mat(SIZE, SIZE);
    multiply(matrix1, SIZE, SIZE, matrix2, SIZE, SIZE, &result);
    free(matrix1);
    free(matrix2);
    free(result);
    printf("Finished 3k\n");
    */
}