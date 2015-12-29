#ifndef BASIC3D_H
#define BASIC3D_H

typedef double scalar;

/* basic matrix functions */

static scalar dot(scalar a[3],scalar b[3]); /* return a.b*/
static scalar* cross(scalar a[3],scalar b[3],scalar c[3]); /* c=axb */
static void normalize(scalar v[3]); /* normalize v */
#define ALLOC(n) calloc(sizeof(scalar),(n))
#define COPY(src,dst,n) memcpy(dst,src,sizeof(scalar)*(n))
#define OP2(A,op,B) 	A[0] op B[0];	A[1] op B[1];	A[2] op B[2];
#define OP3(C,eq,A,op,B) 	C[0] eq A[0] op B[0];	C[1] eq A[1] op B[1];	C[2] eq A[2] op B[2];

scalar* make_rotate(scalar mat[16],scalar x,scalar y, scalar z, scalar ang);
scalar* make_scale(scalar mat[16],scalar sx,scalar sy,scalar sz);
scalar* make_translate(scalar mat[16],scalar tx,scalar ty,scalar tz);
scalar* make_projection(scalar mat[16],scalar fov,scalar near, scalar far, scalar wh);
scalar* make_bias(scalar mat[16])
scalar* invert_mat4x4(scalar mat[16],scalar inv[16]);
scalar* mult_mat4x4(scalar A[16],scalar B[16],scalar C[16]);

#ENDIF
