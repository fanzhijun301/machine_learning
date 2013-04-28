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
	float determ = determinant(matrix);    //���������������ʽ
    float *arr_inverse = inverse_matri->arr;
    if (determ != 0)
    {
        fprintf(stderr, "The determinant of the matrix is %f\n", determ);
        for (i = 0; i < row; i++)    //�������
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

    len = (k-1)*(k-1);            //k�׾���Ĵ�����֮ʽΪk-1�׾���
    p_creat = (float*)calloc(len, sizeof(float)); //�����ڴ浥Ԫ
    if (p_creat == NULL) {
		fprintf(stderr, "malloc memory err\n");
		exit(1);
	}
	p_mid = p_creat;
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < k; j++)
        {
            if (i != m && j != n) //������i�к͵�j���������Ԫ�ش洢����p_midΪ�׵�ַ���ڴ浥Ԫ
            {
                *p_mid++ = *(p+i*k+j);
            }
        }
    }
    sign = (m+n)%2 == 0 ? 1 : -1;    //������֮ʽǰ�����������
    mid_result = (float)sign*Determinant_in(p_creat, k-1);
    free(p_creat);
    return mid_result;
}

//----------------------------------------------
//����: ���ò�����Ԫ�ĸ�˹��ȥ������A�������B
//��ڲ���: ���뷽��������󣬷������
//����ֵ: true or false
//----------------------------------------------
int Gauss(float *A, float *B, size_t n)
{
    size_t i, j, k;
    float max, temp;
	size_t len = n * n;
    float *t = (float *)calloc(len, sizeof(float));                //��ʱ����
    if (t == NULL) {
		fprintf(stderr, "malloc memory err\n");
		return 1;
	}
	//��A����������ʱ����t[n][n]��
    for (i = 0; i < n; i++)        
    {
        for (j = 0; j < n; j++)
        {
			*(t+i*n+j) = *(A+i*n+j);
        }
    }
    //��ʼ��B����Ϊ��λ��
    for (i = 0; i < n; i++)        
    {
        for (j = 0; j < n; j++)
        {
			*(B+i*n+j) = (i == j) ? (float)1 : 0;
        }
    }
    for (i = 0; i < n; i++)
    {
        //Ѱ����Ԫ
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
        //�����Ԫ�����в��ǵ�i�У������н���
        if (k != i)
        {
            for (j = 0; j < n; j++)
            {
                temp = *(t+i*n+j);
                *(t+i*n+j) = *(t+k*n+j);
                *(t+k*n+j) = temp;
                //B���潻��
                temp = *(B+i*n+j);
                *(B+i*n+j) = *(B+k*n+j);
                *(B+k*n+j) = temp;
            }
        }
        //�ж���Ԫ�Ƿ�Ϊ0, ����, �����A�������Ⱦ���,�����������
        if (*(t+i*n+i) == 0)
        {
            return 1;
        }
        //��ȥA�ĵ�i�г�ȥi������ĸ���Ԫ��
        temp = *(t+i*n+i);
        for (j = 0; j < n; j++)
        {
            *(t+i*n+j) = *(t+i*n+j) / temp;        //���Խ����ϵ�Ԫ�ر�Ϊ1
            *(B+i*n+j) = *(B+i*n+j) / temp;        //�������
        }
        for (j = 0; j < n; j++)        //��0��->��n��
        {
            if (j != i)                //���ǵ�i��
            {
                temp = *(t+j*n+i);//t[j][i];
                for (k = 0; k < n; k++)        //��j��Ԫ�� - i��Ԫ��*j��i��Ԫ��
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

