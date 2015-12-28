#ifndef MATRIX_H
#define MATRIX_H

/* 
 * High performanced matrix function-set, where
 * 	1. 1d vectors are treated as nx1 matrix.
 * 	2. All matrix are column-dominated
 */

/* MACROS for debug */

#define MSG(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);
#define EXCEPTION(condition,value,msg) if(condition){printf("\n%s:%d::\t%s",__FILE__,__LINE__,msg); return value;}

/* types */

typedef unsigned int index_t;
typedef double data_t;

/* basic functins */

data_t* data_alloc(index_t n);
void data_destroy(data_t * data);
data_t* data_copy(data_t* src, data_t* dst, index_t n,index_t s_step,index_t d_step); /* function which can used to refine a data aligned */

int data_not_eq(data_t *src,data_t* dst,index_t n);
data_t* data_linear_combine(data_t a,data_t* A, data_t b, data_t* B, data_t* C,index_t n ); /* C=a*A+b*B */

/* basic matrix/vector functions */

typedef data_t* matrix_t;

matrix_t create_matrix(index_t nrow,index_t ncol);
matrix_t create_vector(index_t n);
data_t vector_dot(matrix_t A,matrix_t B,index_t n);
matrix_t matrix_dot_matrix(matrix_t A,matrix_t B,matrix_t C,index_t arow,index_t acol,index_t bcol); /* C=A.B where A is an arow x acol matrix and B is an acol x bcol matrix */
matrix_t matrix_transpose(matrix_t M,matrix_t T,index_t nrow,index_t ncol);

/* linear algebra functions */
matrix_t matrix_inverse(matrix_t M,matrix_t Inv,index_t nrow,index_t ncol);
matrix_t* svd(matrix_t A,int nrow, int ncol);

matrix_t print_matrix(matrix_t data,index_t nrow,index_t ncol);

#endif