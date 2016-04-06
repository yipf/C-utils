
typedef double scalar;
typedef unsigned int uint;

#define ALLOC(n) calloc(sizeof(scalar),(n))
#define COPY(src,dst,n) memcpy(dst,src,sizeof(scalar)*(n))
#define OP2(A,op,B) 	A[0] op B[0];	A[1] op B[1];	A[2] op B[2];
#define OP3(C,eq,A,op,B) 	C[0] eq A[0] op B[0];	C[1] eq A[1] op B[1];	C[2] eq A[2] op B[2];