#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include <stdint.h>

//float *create_random_float_mat(int rows, int cols);
//void free_float_mat(float **mat);
//void print_float_mat(float **mat);

int16_t *create_short_mat(int rows, int cols);
int16_t *create_random_short_mat(int rows, int cols);
void clear_mat(int16_t *mat, int rows, int cols);
void print_short_mat(int16_t *mat, int rows, int cols);

#endif //HELPER_FUNCTIONS_H_
