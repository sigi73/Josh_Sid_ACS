#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

double **create_double_mat();
void fill_double_mat_random(double **mat);
void free_double_mat(double **mat);
void print_double_mat(double **mat);

uint16_t **create_short_mat();
void fill_short_mat_random(uint16_t **mat);
void free_short_mat(uint16_t **mat);
void print_short_mat(uint16_t **mat);

#endif //HELPER_FUNCTIONS_H_
