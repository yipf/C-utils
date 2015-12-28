#include "mygl.h"
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
	if(n>0){n=1/sqrt(n); v[0]=v[0]*n; 	v[1]=v[1]*n;	v[2]=v[2]*n;	}
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

static scalar* mult_mat4x4(scalar A[16],scalar B[16],scalar C[16]){
	unsigned int id,nrow,ncol;
	C=C?C:ALLOC(16);
	for(id=0;id<16;id++){ 
		nrow=id&0x3; 	ncol=id&0xC; 
		C[id]=A[nrow]*B[ncol]+A[nrow+4]*B[ncol+1]+A[nrow+8]*B[ncol+2]+A[nrow+12]*B[ncol+3];	 	/* C[i][j]=A:row(i) . B:col(j), where nrow=i, ncol=j */
	}
	return C;
}

scalar* invert_mat4x4(scalar mat[16],scalar inv[16]){
	scalar *X,*Y,*Z,*T;
	X=mat; 	Y=mat+4;  	Z=mat+8;  	T=mat+12; 
	/* transpose the rotation part */
	inv[0]=X[0];	inv[1]=Y[4];	inv[2]	=Z[8];
	inv[4]=X[1];	inv[5]=Y[5];	inv[6]	=Z[9];
	inv[8]=X[2];	inv[9]=Y[6];	inv[10]	=Z[10];
	/* compute the position part */
	inv[12]=-dot(T,X);	inv[13]=-dot(T,Y);	inv[14]=-dot(T,Z);	inv[15]=1.0;	
	return inv;
}

/* functions for camera */
camera_t create_camera(scalar fov,scalar near,scalar far,scalar wh) { 
	camera_t cam;
	scalar right,top,*mat;
	cam=ALLOC(CAMERA_FULL_SIZE);
	/* initiate the postion and orient */
	make_translate(cam,0.0,0.0,-1.0);
	/* initiate the proj matrix */
	mat=cam+CAMERA_PROJ; 
	top=near*tan(fov/2);	right=wh*top;
	mat[0]=near/right;		mat[5]=near/top;		mat[10]=(far+near)/(near-far);	
	mat[11]=-1.0; 			mat[14]=2*far*near/(near-far);	
	/* initiate the view matrix */
	invert_mat4x4(mat,cam+CAMERA_VIEW);
	/* intiate the bias matrix */
	mat=cam+CAMERA_BIAS;
	mat[0]=0.5;	mat[5]=0.5; mat[10]=0.5;	mat[12]=0.5;	mat[13]=0.5;	mat[14]=0.5;	mat[15]=1.0;
	return cam;
}

camera_t set_camera(camera_t cam,  scalar from[3],scalar to[3], scalar up[3]){
	scalar* X,*Y,*Z;
	X=cam+CAMERA_X; 	Y=cam+CAMERA_Y; 	Z=cam+CAMERA_Z;
	OP3(Z,=,from,-,to);	
	COPY(up,Y,3);
	cross(Y,Z,X); 	cross(Z,X,Y);
	normalize(X); 	normalize(Y);	normalize(Z);
	copy(from,cam+CAMERA_T);	
	invert_mat4x4(cam,cam+CAMERA_VIEW); /* update the view matrix */
	return cam;
}

void apply_camera(camera_t cam){
	/* set projection matrix*/
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(cam+CAMERA_PROJ);
	glMultMatrixd(cam+CAMERAVIEW);
	 /* set modelview matrix*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* draw function for objects */

void set_vertex(scalar vertex[8]); /* vertex=< x,y,z,tx,ty,nx,ny,nz > */
void draw_sphere(uint U,uint V);
void draw_plane(scalar size);
void draw_box(void);

enum{MESH_CELL_POS=0,MESH_CELL_TEX=3,MESH_CELL_NORMAL=5,MESH_CELL_FULL_SIZE=8};
typedef scalar* mesh_t;
uint * create_option(uint nrow,uint ncol,uint uclose,uint vclose){
	uint* option;
	option=calloc(sizeof(uint),4);
	option[0]=nrow; 	option[1]=ncol; 	option[2]=uclose; 	option[3]=vclose;
	return option;
}

mesh_t create_mesh(uint option[4]){
	return ALLOC(((option[0]+option[2])*(option[1]+option[3]))<<3);
}

scalar* get_mesh_cell(mesh_t mesh,uint row,uint col,uint option[4]){
	return mesh+((row*(option[1]+option[3])+col)<<3);
}

void compute_normal(mesh_t mesh,uint option[4]){
	
}

void compute_texture(mesh_t mesh,uint option[4]);
void draw_mesh(mesh_t mesh,uint option[4]);