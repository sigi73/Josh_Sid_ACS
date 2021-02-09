#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "helper_functions.h"

#define RAND_SEED 0xdeadbeef

int16_t *create_short_mat(int rows, int cols)
{
    // TODO: Handle calloc failure
    int16_t *matrix = calloc(rows * cols, sizeof(int16_t));
    return matrix;
}

int16_t *create_random_short_mat(int rows, int cols)
{
    srand(RAND_SEED);

    int16_t *matrix = create_short_mat(rows, cols);
    for (int i = 0; i < rows * cols; i++)
    {
        int rand_num = rand();
        matrix[i] = (rand() % (SHRT_MAX + 1 - SHRT_MIN)) + SHRT_MIN; 
    }
    return matrix;
}

void print_short_mat(int16_t *mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d,", mat[i*cols + j]);
        }
        printf("\n");
    }
}