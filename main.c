
#include "math/matrix.h"

#include <stdio.h>



typedef data_t * tensor_t;

int main(){
	//~ tensor_t m1,m2,m3,v1,v2;
	//~ data_t v=1.0;
	//~ index_t nrow=4,ncol=3;
	//~ m1=data_alloc(nrow*ncol);
	//~ v1=data_alloc(ncol);
	//~ data_copy(&v,m1,nrow*ncol,0,1);
	//~ data_copy(&v,v1,ncol,0,1);
	//~ print_matrix(m1,nrow,ncol);
	//~ print_matrix(v1,1,ncol);
	//~ m2=matrix_transpose(m1,0,nrow,ncol);
	//~ print_matrix(m2,ncol,nrow);
	//~ v2=matrix_dot_matrix(m1,v1,0,nrow,ncol,1); 
	//~ print_matrix(v2,nrow,1);
	//~ m3=data_copy(m1,0,ncol,nrow,1);
	//~ print_matrix(m3,ncol,1);
	/* https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_dgesvd_row.c.htm */
	data_t m[]={
		 8.79,  9.93,  9.83, 5.45,  3.16,
            6.11,  6.91,  5.04, -0.27,  7.98,
           -9.15, -7.93,  4.86, 4.85,  3.01,
            9.57,  1.64,  8.83, 0.74,  5.80,
           -3.49,  4.02,  9.80, 10.00,  4.27,
            9.84,  0.15, -8.99, -6.02, -5.31
	};
	matrix_t T,*R;
	T=matrix_transpose(m,0,5,6);
	print_matrix(T,6,5);
	R=svd(T,6,5);
	printf("\nU:");	print_matrix(R[0],6,6);
	printf("\nS:");	print_matrix(R[1],5,1);
	printf("\nVT:");	print_matrix(R[2],5,5);
	return 0;
}