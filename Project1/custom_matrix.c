#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <unistd.h>

#include "helper_functions.h"
#include "matrix_multiply.h"

void Custom_Matrix_Gen(){
    int Col1_Row2 = 0; // same
    int Row1 = 0; // Row of product
    int Col2 = 0; // Col of Product
    int int_float = 0; //0 is int, 1 is float
    int SIMD_reg = 0; //0 is SIMD, 1 is regular
    int naive_block= 0; // 0 is Naive, 1 is Block
    int block_size= 0; 
    struct timeval time_start;
	struct timeval time_end;
    printf("Enter number of Columns of Matrix A\n");
    scanf("%d", &Col1_Row2);  
    printf("Enter number of Rows of Matrix A\n");
    scanf("%d", &Row1);  
    printf("Enter number of Rows of Matrix B\n");
    scanf("%d", &Col2);  
    printf("Int(0) or Float(1)\n");
    scanf("%d", &int_float); 
    printf("SIMD(0) or Regular(1)\n");
    scanf("%d", &SIMD_reg); 
    // if regular
    if (SIMD_reg){
        printf("Naive(0) or Block(1)\n");
        scanf("%d", &naive_block); 
        //if block, what size block?
        if(naive_block){
            printf("Enter Block Size\n");
            scanf("%d", &block_size); 
            //making sure block size is within bounds
            if(block_size > Row1 || block_size > Col2){
                block_size == Row1;
            }
        }
    } 
    if(int_float){
         float *matrixf1 = create_random_float_mat(Row1, Col1_Row2);
         float *matrixf2 = create_random_float_mat(Col1_Row2, Col2);
         float *resultf = create_float_mat(Row1, Col2);
         if(SIMD_reg){
             if (naive_block){//float regular block
                printf("Starting %d x %d * %d x %d float block\n", Row1, Col1_Row2,Col1_Row2,Col2 );
             	gettimeofday(&time_start, NULL);
                multiply_float_block(matrixf1, Row1, Col1_Row2, matrixf2, Col1_Row2, Col2, block_size, resultf);
                gettimeofday(&time_end, NULL);
             }
             else{//float regular naive
                printf("Starting %d x %d * %d x %d float naive\n", Row1, Col1_Row2,Col1_Row2,Col2 );
                gettimeofday(&time_start, NULL);
                multiply_float_naive(matrixf1, Row1, Col1_Row2, matrixf2, Col1_Row2, Col2, resultf);
                gettimeofday(&time_end, NULL);
             }
         }
        else{
            printf("Starting %d x %d * %d x %d float SIMD\n", Row1, Col1_Row2,Col1_Row2,Col2 );
            gettimeofday(&time_start, NULL);
            multiply_float_simd( matrixf1, Row1, Col1_Row2, matrixf2, Col1_Row2, Col2, resultf);
            gettimeofday(&time_end, NULL);
         }
        free(matrixf1);
        free(matrixf2);
        free(resultf);
    }
    else{
        int16_t *matrix1 = create_random_short_mat(Row1, Col1_Row2);
        int16_t *matrix2 = create_random_short_mat(Col1_Row2, Col2);
        int16_t *result = create_short_mat(Row1, Col2);
        if(SIMD_reg){
             if (naive_block){//int regular block
                printf("Starting %d x %d * %d x %d int block\n", Row1, Col1_Row2,Col1_Row2,Col2 );
                gettimeofday(&time_start, NULL);
                multiply_short_block(matrix1, Row1, Col1_Row2, matrix2, Col1_Row2, Col2, block_size, result);
                gettimeofday(&time_end, NULL);

             }
             else{//int regular naive
                printf("Starting %d x %d * %d x %d int naive\n", Row1, Col1_Row2,Col1_Row2,Col2 );
                gettimeofday(&time_start, NULL);
                multiply_short_naive(matrix1, Row1, Col1_Row2, matrix2, Col1_Row2, Col2, result);
                gettimeofday(&time_end, NULL);
             }
         }
        else{
            printf("Starting %d x %d * %d x %d int SIMD\n", Row1, Col1_Row2,Col1_Row2,Col2 );
            gettimeofday(&time_start, NULL);
            multiply_short_simd(matrix1, Row1, Col1_Row2, matrix2, Col1_Row2, Col2, result);
            gettimeofday(&time_end, NULL);
         }
        free(matrix1);
        free(matrix2);
        free(result);
    }
    long long multiply_time = 1000000LL * (time_end.tv_sec  - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec);
	printf("time for completion: %lld\n", multiply_time);

}

void main()
{
    while(1){
        Custom_Matrix_Gen();
    }
}

