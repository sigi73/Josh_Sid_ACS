#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "helper_functions.h"

#define RAND_SEED 0xdeadbeef

//creates empty matrix of 2 byte ints of size rows x cols
int16_t *create_short_mat(int rows, int cols)
{
    int16_t *matrix = calloc(rows * cols, sizeof(int16_t));
    return matrix;
}

//creates filled matrix of 2 byte ints of size rows x cols
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
//generate empty matrix of float
float *create_float_mat(int rows, int cols)
{
    float *matrix = calloc(rows * cols, sizeof(float));
    return matrix;
}

//creates filled matrix of floatr of size rows x cols
float *create_random_float_mat(int rows, int cols)
{
    srand(RAND_SEED);
    float a = 20.0; //arbitrary value a
    float *matrix = create_float_mat(rows, cols);
    for (int i = 0; i < rows * cols; i++)
    {
        //generates random number between 0 and a
        float rand_num = ((float)rand()/(float)(RAND_MAX)) * a;
        matrix[i] = rand_num; 
    }
    return matrix;
}


//prints matrix populated by 2 byte ints of size rows x cols
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

//prints matrix populated by floats of size rows x cols
void print_float_mat(float *mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%.6f,", mat[i*cols + j]);
        }
        printf("\n");
    }
}

//clears matrix of floats of size rows x cols
void clear_float_mat(float *mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat[i*cols + j] = 0;
        }
    }
}
//clears matrix of 2 byte ints of size rows x cols
void clear_mat(int16_t *mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat[i*cols + j] = 0;
        }
    }
}

