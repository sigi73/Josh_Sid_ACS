#ifndef MATRIX_MULTIPLY_H
#define MATRIX_MULTIPLY_H

#include <stdint.h>


void multiply_short_naive(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int16_t *result);
void multiply_short_block(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int block_size, int16_t *result);
void multiply_float_naive(float *m1, int rows1, int cols1, float *m2, int rows2, int cols2, float *result);
void multiply_float_block(float *m1, int rows1, int cols1, float *m2, int rows2, int cols2, int block_size, float *result);
void multiply_short_simd(int16_t *m1, int rows1, int cols1, int16_t *m2, int rows2, int cols2, int16_t *result);

#endif // MATRIX_MULTIPLY_H