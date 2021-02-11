#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <immintrin.h>

#include "matrix_multiply.h"

void multiply_short_naive(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int16_t *result) 
{
    if (cols1 != rows2)
    {
        // TODO: Add error message
        exit(1);
    }

    for (int i = 0; i < rows1; i++)
    {
        for (int k = 0; k < cols1; k++)
        {
            for (int j = 0; j < cols2; j++)
            {
                result[i * cols2 + j] += m1[i*cols1 + k] * m2[k*cols2 + j];
            }
        }
    }
}
void multiply_float_naive(float *m1, int rows1, int cols1, float *m2, int rows2, int cols2, float *result)
{
    if (cols1 != rows2)
    {
        // TODO: Add error message
        exit(1);
    }

    for (int i = 0; i < rows1; i++)
    {
        for (int k = 0; k < cols1; k++)
        {
            for (int j = 0; j < cols2; j++)
            {
                result[i * cols2 + j] += m1[i*cols1 + k] * m2[k*cols2 + j];
            }
        }
    }
}

void multiply_short_block(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int block_size, int16_t *result)
{
    if (cols1 != rows2)
    {
        // TODO: Add error message
        exit(1);
    }

    // Todo: Create diagram showing how multiplying by block works
    for (int j = 0; j < cols2; j += block_size)
    {
        for (int k = 0; k < cols1; k += block_size)
        {
            for (int i = 0; i < rows1; i++)
            {
                int max_k = fmin(k + block_size, cols1);
                for (int k2 = k; k2 < max_k; k2++)
                {
                    int max_j = fmin(j + block_size, cols2);
                    for (int j2 = j; j2 < max_j; j2++)
                    {
                        result[i * cols2 + j2] += m1[i * cols1 + k2] * m2[k2 * cols2 + j2];
                    }
                }
            }
        }
    }
}
void multiply_float_block(float *m1, int rows1, int cols1, float *m2, int rows2, int cols2, int block_size, float *result)
{
    if (cols1 != rows2)
    {
        // TODO: Add error message
        exit(1);
    }

    // Todo: Create diagram showing how multiplying by block works
    for (int j = 0; j < cols2; j += block_size)
    {
        for (int k = 0; k < cols1; k += block_size)
        {
            for (int i = 0; i < rows1; i++)
            {
                int max_k = fmin(k + block_size, cols1);
                for (int k2 = k; k2 < max_k; k2++)
                {
                    int max_j = fmin(j + block_size, cols2);
                    for (int j2 = j; j2 < max_j; j2++)
                    {
                        result[i * cols2 + j2] += m1[i * cols1 + k2] * m2[k2 * cols2 + j2];
                    }
                }
            }
        }
    }
}


void multiply_short_simd(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int16_t *result)
{
    if (cols1 != rows2)
    {
        // TODO: Add error message
        exit(1);
    }

    int simd_width = 256/16; //256 bit SIMD, 16 bit integers

    for (int i = 0; i < rows1; i++)
    {
        for (int k = 0; k < cols1; k++)
        {
            int j;
            // 16*floor(cols/16)
            int j_max = (cols2 >> 4) << 4;
            for (j = 0; j < j_max; j += simd_width)
            {
                //result[i * cols2 + j] += m1[i*cols1 + k] * m2[k*cols2 + j];
                __m256i out = _mm256_loadu_si256((__m256i *)(result + i*cols2 + j));
                __m256i in1 = _mm256_loadu_si256((__m256i *)(m2 + k*cols2 + j));
                __m256i in2 = _mm256_set1_epi16(m1[i*cols1 + k]);
                out = _mm256_add_epi16(out, _mm256_mullo_epi16(in1, in2));
                _mm256_storeu_si256((__m256i *)(result + i*cols2 + j), out);
            }
            for (; j < cols2; j ++)
            {
                result[i * cols2 + j] += m1[i*cols1 + k] * m2[k*cols2 + j];
            }
        }
    }
}
