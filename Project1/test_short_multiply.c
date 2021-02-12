#include <stdio.h>
#include <stdlib.h>

#include "helper_functions.h"
#include "matrix_multiply.h"

int16_t test_case_2_output[] = {
0,  136,  272,  408,  544,  680,  816,  952, 1088, 1224, 1360, 1496, 1632, 1768, 1904, 2040, 2176,    0,  425,  850, 1275, 1700, 2125, 2550, 2975, 3400, 3825, 4250, 4675, 5100, 5525, 5950, 6375, 6800,    0,  714, 1428, 2142, 2856, 3570, 4284, 4998, 5712, 6426, 7140, 7854, 8568, 9282, 9996,10710,11424,    0, 1003, 2006, 3009, 4012, 5015, 6018, 7021, 8024, 9027,10030,11033,12036,13039,14042,15045,16048,    0, 1292, 2584, 3876, 5168, 6460, 7752, 9044,10336,11628,12920,14212,15504,16796,18088,19380,20672,    0, 1581, 3162, 4743, 6324, 7905, 9486,11067,12648,14229,15810,17391,18972,20553,22134,23715,25296,    0, 1870, 3740, 5610, 7480, 9350,11220,13090,14960,16830,18700,20570,22440,24310,26180,28050,29920,    0, 2159, 4318, 6477, 8636,10795,12954,15113,17272,19431,21590,23749,25908,28067,30226,32385,34544,    0, 2448, 4896, 7344, 9792,12240,14688,17136,19584,22032,24480,26928,29376,31824,34272,36720,39168,    0, 2737, 5474, 8211,10948,13685,16422,19159,21896,24633,27370,30107,32844,35581,38318,41055,43792,    0, 3026, 6052, 9078,12104,15130,18156,21182,24208,27234,30260,33286,36312,39338,42364,45390,48416,    0, 3315, 6630, 9945,13260,16575,19890,23205,26520,29835,33150,36465,39780,43095,46410,49725,53040,    0, 3604, 7208,10812,14416,18020,21624,25228,28832,32436,36040,39644,43248,46852,50456,54060,57664,    0, 3893, 7786,11679,15572,19465,23358,27251,31144,35037,38930,42823,46716,50609,54502,58395,62288,    0, 4182, 8364,12546,16728,20910,25092,29274,33456,37638,41820,46002,50184,54366,58548,62730,66912,    0, 4471, 8942,13413,17884,22355,26826,31297,35768,40239,44710,49181,53652,58123,62594,67065,71536,    0, 4760, 9520,14280,19040,23800,28560,33320,38080,42840,47600,52360,57120,61880,66640,71400,76160
};

int check_short_mat_equal(int16_t *m1, int16_t *m2, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (m1[i*cols + j] != m2[i*cols + j]) return 0;
        }
    }
    return 1;
}

int test_mult(int16_t *m1, int r1, int c1, int16_t *m2, int r2, int c2, int block_size, int16_t *expected_result)
{
    int16_t *result = create_short_mat(r1, c2);
    int res = 1;

    multiply_short_naive(m1, r1, c1, m2, r2, c2, result);
    res &= check_short_mat_equal(result, expected_result, r1, c2);
    if (res == 0)
    {
        fprintf(stderr, "Naive failed\n");
    }
    clear_mat(result, r1, c2);

    multiply_short_block(m1, r1, c1, m2, r2, c2, block_size, result);
    res &= check_short_mat_equal(result, expected_result, r1, c2);
    if (res == 0)
    {
        fprintf(stderr, "Block failed\n");
    }
    clear_mat(result, r1, c2);

    multiply_short_simd(m1, r1, c1, m2, r2, c2, result);
    res &= check_short_mat_equal(result, expected_result, r1, c2);
    if (res == 0)
    {
        fprintf(stderr, "SIMD Failed\n");
    }
    clear_mat(result, r1, c2);

    return res;
}

// Test short matrix matrix multiplication
void main()
{
    int SIZE;
    int16_t *m1, *m2, *expected_result; 
    int output;

    SIZE = 17;

    m1 = create_short_mat(SIZE, SIZE);
    m2 = create_short_mat(SIZE, SIZE);
    expected_result = create_short_mat(SIZE, SIZE);

    for (int i = 0; i < SIZE*SIZE; i++)
    {
        m1[i] = 1;
        m2[i] = 2;
        expected_result[i] = 34;
    }
    output = test_mult(m1, SIZE, SIZE, m2, SIZE, SIZE, 4, expected_result);
    if (output == 0)
    {
        printf("Error in test case 1\n");
    }
    else
    {
        printf("Test case 1 passed\n");
    }

    for (int i = 0; i < SIZE*SIZE; i++)
    {
        m1[i] = i;
        m2[i] = i % SIZE;
    }
    output = test_mult(m1, SIZE, SIZE, m2, SIZE, SIZE, 4, test_case_2_output);
    if (output == 0)
    {
        printf("Error in test case 2\n");
    }
    else
    {
        printf("Test case 2 passed\n");
    }

    free(m1);
    free(m2);
    free(expected_result);
}