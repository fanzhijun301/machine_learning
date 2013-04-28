#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "matrix.h"

int init_matrix(Matrix *matrix, size_t row_len, size_t col_len) {
	float *arr = (float *)calloc(row_len * col_len, sizeof(float));
	if (arr == NULL) {
		fprintf(stderr, "malloc memory err\n");
		exit(1);
	}
	matrix->arr = arr;
	matrix->row_len = row_len;
	matrix->col_len = col_len;
	
	return 0;
}

int release_matrix(Matrix *matrix) {
	float *arr = matrix->arr;
	free(arr);
	return 0;
}

float get_value(Matrix *matrix, size_t row, size_t col) {
	float *arr = matrix->arr;
	float d = *(arr + row * matrix->col_len + col);
	return d;
}

int set_value(Matrix *matrix, size_t row, size_t col, float d) {
	float *arr = matrix->arr;
	*(arr + row * matrix->col_len + col) = d;
	return 0;
}

int print_matrix(Matrix *matrix) {
	size_t i,j;
	for (i = 0; i < matrix->row_len; i++) {
		for (j = 0; j < matrix->col_len; j++) {
			float d = get_value(matrix, i, j);
			if (j == 0) printf("%f", d);
			else printf(" %f", d);
		}
		printf("\n");
	}
	return 0;
}

int inverse(Matrix *matrix, Matrix *inverse_matri) {
	
	size_t n = matrix->col_len;
	init_matrix(inverse_matri, n, n);
	float *arr = matrix->arr;
	float *inver_arr = inverse_matri->arr;
	Gauss(arr, inver_arr, n);
	//inverse_define(matrix, inverse_matri);
	
	return 0;
}

int inverse_define(Matrix *matrix, Matrix *inverse_matri) {
	size_t i, j;
	size_t row = matrix->row_len;
	float *arr = matrix->arr;
	float determ = determinant(matrix);    //求整个矩阵的行列式
    float *arr_inverse = inverse_matri->arr;
    if (determ != 0)
    {
        fprintf(stderr, "The determinant of the matrix is %f\n", determ);
        for (i = 0; i < row; i++)    //求逆矩阵
        {
            for (j = 0; j < row; j++)
            {
                *(arr_inverse + j * row + i) = Creat_M(arr, i, j, row)/determ;
            }
        }
    }
    else
    {
        fprintf(stderr, "The determinant is 0, and there is no inverse matrix!\n");
    }
	return 0;
}

float determinant(Matrix *matrix) {

	float *arr = matrix->arr;
	size_t row_col = matrix->row_len;
	float deter = Determinant_in(arr, row_col);
	return deter;
}

float Determinant_in(float *a, size_t n)
{
	size_t i,j,j1,j2;
	float det = 0;
	float *m = NULL;

	if (n < 1) { /* Error */
		fprintf(stderr, "n is low 1\n");
	} else if (n == 1) { /* Shouldn't get used */
		det = *(a + 0 * n + 0);
	} else if (n == 2) {
		det = *a * *(a + 1 * n + 1) - *(a + 1 * n) * *(a + 1);
	} else {
		det = 0;
		for (j1=0;j1<n;j1++) {
			m = (float *)calloc((n - 1) * (n - 1), sizeof(float *));
			if (m == NULL) {
				fprintf(stderr, "malloc memory err\n");
				exit(1);
			}
			for (i=1;i<n;i++) {
				j2 = 0;
				for (j=0;j<n;j++) {
					if (j == j1)
						continue;
					*(m + (n - 1) * (i - 1) + j2) = *(a + i * n + j);
					j2++;
				}
			}
			det += pow(-1.0,1.0+j1+1.0) * *(a + j1) * Determinant_in(m, n-1);
			free(m);
		}
	}
	return det;
}

float Creat_M(float *p, int m, int n, int k)
{
    int len;
    int i, j;
    float mid_result = 0;
    int sign = 1;
    float *p_creat, *p_mid;

    len = (k-1)*(k-1);            //k阶矩阵的代数余之式为k-1阶矩阵
    p_creat = (float*)calloc(len, sizeof(float)); //分配内存单元
    if (p_creat == NULL) {
		fprintf(stderr, "malloc memory err\n");
		exit(1);
	}
	p_mid = p_creat;
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < k; j++)
        {
            if (i != m && j != n) //将除第i行和第j列外的所有元素存储到以p_mid为首地址的内存单元
            {
                *p_mid++ = *(p+i*k+j);
            }
        }
    }
    sign = (m+n)%2 == 0 ? 1 : -1;    //代数余之式前面的正、负号
    mid_result = (float)sign*Determinant_in(p_creat, k-1);
    free(p_creat);
    return mid_result;
}

//----------------------------------------------
//功能: 采用部分主元的高斯消去法求方阵A的逆矩阵B
//入口参数: 输入方阵，输出方阵，方阵阶数
//返回值: true or false
//----------------------------------------------
int Gauss(float *A, float *B, size_t n)
{
    size_t i, j, k;
    float max, temp;
	size_t len = n * n;
    float *t = (float *)calloc(len, sizeof(float));                //临时矩阵
    if (t == NULL) {
		fprintf(stderr, "malloc memory err\n");
		return 1;
	}
	//将A矩阵存放在临时矩阵t[n][n]中
    for (i = 0; i < n; i++)        
    {
        for (j = 0; j < n; j++)
        {
			*(t+i*n+j) = *(A+i*n+j);
        }
    }
    //初始化B矩阵为单位阵
    for (i = 0; i < n; i++)        
    {
        for (j = 0; j < n; j++)
        {
			*(B+i*n+j) = (i == j) ? (float)1 : 0;
        }
    }
    for (i = 0; i < n; i++)
    {
        //寻找主元
        max = *(t+i*n+i);//t[i][i];
        k = i;
        for (j = i+1; j < n; j++)
        {
            if (fabs(*(t+j*n+i)) > fabs(max))
            {
                max = *(t+j*n+i);
                k = j;
            }
        }
        //如果主元所在行不是第i行，进行行交换
        if (k != i)
        {
            for (j = 0; j < n; j++)
            {
                temp = *(t+i*n+j);
                *(t+i*n+j) = *(t+k*n+j);
                *(t+k*n+j) = temp;
                //B伴随交换
                temp = *(B+i*n+j);
                *(B+i*n+j) = *(B+k*n+j);
                *(B+k*n+j) = temp;
            }
        }
        //判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
        if (*(t+i*n+i) == 0)
        {
            return 1;
        }
        //消去A的第i列除去i行以外的各行元素
        temp = *(t+i*n+i);
        for (j = 0; j < n; j++)
        {
            *(t+i*n+j) = *(t+i*n+j) / temp;        //主对角线上的元素变为1
            *(B+i*n+j) = *(B+i*n+j) / temp;        //伴随计算
        }
        for (j = 0; j < n; j++)        //第0行->第n行
        {
            if (j != i)                //不是第i行
            {
                temp = *(t+j*n+i);//t[j][i];
                for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素
                {
                    *(t+j*n+k) = *(t+j*n+k) - *(t+i*n+k) * temp;
                    *(B+j*n+k) = *(B+j*n+k) - *(B+i*n+k) * temp;
                }
            }
        }
    }
	free(t);
    return 0;
}

int transpose(Matrix *matrix, Matrix *trans_matrix) {
	int row_len = matrix->row_len;
	int col_len = matrix->col_len;
	init_matrix(trans_matrix, col_len, row_len);

	float *arr = matrix->arr;
	float *trans_arr = trans_matrix->arr;
	int i, j;
	for (i = 0; i < row_len; i++) {
		for (j = 0; j < col_len; j++) {
			float d = *(arr + i * col_len + j);
			*(trans_arr + j * row_len + i) = d;
		}
	}
	return 0;
}

int multiple(Matrix *matr_a, Matrix *matr_b, Matrix *matr_re) {
	size_t a_row = matr_a->row_len;
	size_t a_col = matr_a->col_len;
	size_t b_col = matr_b->col_len;

	float *a_arr = matr_a->arr;
	float *b_arr = matr_b->arr;
	init_matrix(matr_re, a_row, b_col);
	float *re_arr = matr_re->arr;
	for (size_t i = 0; i < a_row; i++) {
		for (size_t j = 0; j < b_col; j++) {
			float re_i_j = 0;
			for (size_t k = 0; k < a_col; k++) {
				float a_i_k = *(a_arr + i * a_col + k);
				float b_k_j = *(b_arr + k * b_col + j);
				re_i_j += a_i_k * b_k_j;
			}
			*(re_arr + i * b_col + j) = re_i_j;
		}
	}

	return 0;
}

