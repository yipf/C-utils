#include "matrix.h"
/* calloc */
#include <stdlib.h>
/* printf */
#include <stdio.h>
/* memcpy */
#include <string.h> 
/* Lapack functions */
#include <lapacke.h>

data_t* data_alloc(index_t n){
	return calloc(sizeof(data_t),n);
}

void data_destroy(data_t* data){
	if(data){free(data);}
}

data_t* data_copy(data_t* src, data_t* dst, index_t n,index_t s_step,index_t d_step){
	EXCEPTION(!src,src,"Data source must be valid!") 
	if(!dst) {dst=data_alloc(n*d_step);}
	if(s_step){
		if(s_step==1&&d_step==1){ memcpy(dst,src,n*sizeof(data_t)); }
		else if(d_step==1) { while(n--){memcpy(dst+n,src+n*s_step,sizeof(data_t));} }
		else if(s_step==1) { while(n--){memcpy(dst+n*d_step,src+n,sizeof(data_t));} }
		else{	while(n--){memcpy(dst+n*d_step,src+n*s_step,sizeof(data_t));} } 
	}else{
		if(d_step==1){while(n--){memcpy(dst+n,src,sizeof(data_t));}}
		else{	while(n--){memcpy(dst+n*d_step,src,sizeof(data_t));}	}
	}
	return dst;
}

int data_not_eq(data_t *src,data_t* dst,index_t n){
	if(src-dst) {while(n--){  if(src[n]-dst[n]) return 1;} }
	return 0;
}

data_t* data_linear_combine(data_t a,data_t* A, data_t b, data_t* B, data_t* C ,index_t n){
	index_t count;
	C=C?C:data_alloc(n);
	count=n; 	while(count--){ C[count]=a*A[count];}  /* C=a*A */
	if(B&&b){count=n; 	while(count--){ C[count]+=b*B[count];}} /* C+=b*B */
	return C;
}

matrix_t create_matrix(index_t nrow,index_t ncol){
	return data_alloc(nrow*ncol);
}
matrix_t create_vector(index_t n){
	return data_alloc(n);
}

data_t vector_dot(matrix_t A,matrix_t B,index_t n){
	data_t sum=0;
	while(n--){sum+=A[n]*B[n];}
	return sum;
}

matrix_t matrix_dot_matrix(matrix_t A,matrix_t B,matrix_t C,index_t arow,index_t acol,index_t bcol){
	index_t i,j;	data_t sum,*a,*c;
	C=C?C:create_matrix(arow,bcol);
	c=C;
	while(bcol--){	
		for(i=0;i<arow;i++){		sum=0.0; a=A+i;	 for(j=0;j<acol;j++){ sum+=a[j*arow]*B[j];}		c[i]=sum; }
		B+=acol; 	c+=arow;	
	}
	return C;
}

matrix_t matrix_transpose(matrix_t M,matrix_t T,index_t nrow,index_t ncol){
	index_t count;
	T=T?T:create_matrix(ncol,nrow); 																				/* T is ncol x nrow*/
	count=nrow; while(count--){data_copy(M+count,T+count*ncol,ncol,nrow,1);} /* T:col(count)=M:row(count)*/
	return T;
}

matrix_t* svd(matrix_t A,int nrow, int ncol){ 
	matrix_t U,S,VT,superb,*R;
	U=create_matrix(nrow,nrow); 	S=create_vector(ncol); 	VT=create_matrix(ncol,ncol); 	superb=create_vector(((nrow>ncol)?ncol:nrow)-1);
	/* lapack_int LAPACKE_dgesvd( int matrix_layout, char jobu, char jobvt,lapack_int m, lapack_int n, double* a, lapack_int lda, double* s, double* u, lapack_int ldu, double* vt, lapack_int ldvt, double* superb )   */
	EXCEPTION(LAPACKE_dgesvd( LAPACK_COL_MAJOR, 'A','A',nrow,ncol,A,nrow,S,U,nrow,VT,ncol,superb),0,"SVD processing faild to complete!\n");
	R=calloc(sizeof(matrix_t),4);
	R[0]=U; 	R[1]=S; 	R[2]=VT; 	R[3]=superb;
	return R;
}

matrix_t matrix_inverse(matrix_t M,matrix_t Inv,index_t nrow,index_t ncol){
	return 0;
}

matrix_t print_matrix(matrix_t data,index_t nrow,index_t ncol){
	index_t i,j;
	printf("\nMatrix (%dx%d)=",nrow,ncol);
	for(i=0;i<nrow;i++){
		printf("\n[");for(j=0;j<ncol;j++){ 	printf("%12lf",data[i+j*nrow]);}printf("\t]");
	}
	printf("\n");
	return data;
}




