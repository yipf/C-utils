#include "tensor3d.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* basic matrix functions */

scalar dot(scalar a[3],scalar b[3]){ 	
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

scalar* cross(scalar a[3],scalar b[3],scalar c[3]){
	scalar a1,a2,a3,b1,b2,b3;
	a1=a[0]; 	a2=a[1]; 	a3=a[2];
	b1=b[0]; 	b2=b[1]; 	b3=b[2];
	c[0]=a2*b3-a3*b2; 	c[1]=a3*b1-b3*a1; 	c[2]=a1*b2-a2*b1;
	return c;
}

void normalize(scalar v[3]){
	scalar n;
	n=dot(v,v);
	if(n>0.0){n=1/sqrt(n); v[0]=v[0]*n; 	v[1]=v[1]*n;	v[2]=v[2]*n;	}
}

scalar* make_rotate(scalar mat[16],scalar x,scalar y, scalar z, scalar ang){
	scalar n,s,c;
	n=sqrt(x*x+y*y+z*z);	s=sin(ang);	c=cos(ang);
	x=x/n;	y=y/n;	z=z/n;	
	mat=mat?mat:ALLOC(16);
	mat[0]=x*x*(1-c)+c;		mat[4]=x*y*(1-c)-z*s;		mat[8]=x*z*(1-c)+y*s;	
	mat[1]=x*y*(1-c)+z*s;	mat[5]=y*y*(1-c)+c;		mat[9]=y*z*(1-c)-x*s;	 
	mat[2]=x*z*(1-c)-y*s;		mat[6]=y*z*(1-c)+x*s;	mat[10]=z*z*(1-c)+c;	
	mat[15]=1.0;
	return mat;
}

scalar* make_scale(scalar mat[16],scalar sx,scalar sy,scalar sz){
	mat=mat?mat:ALLOC(16);
	mat[0]=sx;	mat[5]=sy;	mat[10]=sz; mat[15]=1.0;
	return mat;
}

scalar* make_translate(scalar mat[16],scalar tx,scalar ty,scalar tz){
	mat=mat?mat:ALLOC(16);
	mat[0]=1;	mat[5]=1;	mat[10]=1;	
	mat[12]=tx;	mat[13]=ty;	mat[14]=tz;	mat[15]=1.0;
	return mat;
}

scalar* make_projection(scalar mat[16],scalar fov,scalar near, scalar far, scalar wh){
	scalar right,top;
	mat=mat?mat:ALLOC(16);
	top=near*tan(fov/2);	right=wh*top;
	mat[0]=near/right;		mat[5]=near/top;		mat[10]=(far+near)/(near-far);	
	mat[11]=-1.0; 			mat[14]=2*far*near/(near-far);	
	return mat;
}

scalar* make_bias(scalar mat[16]){
	mat=mat?mat:ALLOC(16);
	mat[0]=0.5;	mat[5]=0.5; mat[10]=0.5;	
	mat[12]=0.5;	mat[13]=0.5;	mat[14]=0.5;	mat[15]=1.0;
	return mat;
}

scalar* make_point_to(scalar mat[16],scalar x,scalar y,scalar z,scalar sx,scalar sy){
	scalar* X,*Y,*Z;
	mat=mat?mat:ALLOC(16);
	X=mat; 	Y=mat+4; 	Z=mat+8;
	Z[0]=x;	Z[1]=y;	Z[2]=z;	
	Y[2]=0.0;
	if(x==0){Y[0]=1.0;}else{Y[1]=1.0;}
	cross(Y,Z,X); 	cross(Z,X,Y);
	normalize(X); 	normalize(Y); 	normalize(Z); 	
	X[0]*=sx; 	X[1]*=sx; 	X[2]*=sx;
	Y[0]*=sy; 	Y[1]*=sy; 	Y[2]*=sy;
	sx=sqrt(x*x+y*y+z*z)/2.0;
	Z[0]*=sx; Z[1]*=sx; Z[2]*=sx;
	mat[12]=x/2.0;	mat[13]=y/2.0;	mat[14]=z/2.0;	mat[15]=1.0;
	return mat;
}

static scalar R[16];

scalar* mult_mat4x4(scalar A[16],scalar B[16],scalar C[16]){
	uint id,nrow,ncol;
	for(id=0;id<16;id++){ 
		nrow=id&0x3; 	ncol=id&0xC; 
		R[id]=A[nrow]*B[ncol]+A[nrow+4]*B[ncol+1]+A[nrow+8]*B[ncol+2]+A[nrow+12]*B[ncol+3];	 	/* C[i][j]=A:row(i) . B:col(j), where nrow=i, ncol=j */
	}
	C=C?C:ALLOC(16);
	memcpy(C,R,16*sizeof(scalar));
	return C;
}

scalar* invert_mat4x4(scalar mat[16],scalar inv[16]){
	scalar *X,*Y,*Z,*T;
	X=mat; 	Y=mat+4;  	Z=mat+8;  	T=mat+12; 
	/* transpose the rotation part */
	inv[0]=X[0];	inv[1]=Y[0];	inv[2]	=Z[0];
	inv[4]=X[1];	inv[5]=Y[1];	inv[6]	=Z[1];
	inv[8]=X[2];	inv[9]=Y[2];	inv[10]	=Z[2];
	/* compute the position part */
	inv[12]=-dot(T,X);	inv[13]=-dot(T,Y);	inv[14]=-dot(T,Z);	inv[15]=1.0;	
	return inv;
}

