#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdlib.h>

typedef struct Matrix {
	size_t row_len;
	size_t col_len;
	float *arr;
} Matrix;

int init_matrix(Matrix *matrix, size_t row_len, size_t col_len);

int release_matrix(Matrix *matrix);

float get_value(Matrix *matrix, size_t row, size_t col);

int set_value(Matrix *matrix, size_t row, size_t col, float d);

int print_matrix(Matrix *matrix);

float determinant(Matrix *matrix);

float Determinant_in(float *a, size_t n);

int inverse(Matrix *matrix, Matrix *inverse_matri);

float Creat_M(float *p, int m, int n, int k);

int transpose(Matrix *matrix);

int multiple(Matrix *matr_a, Matrix *matr_b, Matrix *matr_re);

bool Gauss(float *A, float *B, size_t n);

int inverse_define(Matrix *matrix, Matrix *inverse_matri);
#endif
