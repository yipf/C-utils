#include "camera.h"
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
	scalar *mat;
	cam=ALLOC(CAMERA_FULL_SIZE);	
	make_translate(cam,0.0,0.0,1.0);	/* initiate the postion and orient */
	make_projection(cam+CAMERA_PROJ,fov,near,far,wh);	/* initiate the proj matrix */
	invert_mat4x4(mat,cam+CAMERA_VIEW);/* initiate the view matrix */
	make_bias(cam+CAMERA_BIAS);/* intiate the bias matrix */
	return cam;
}

camera_t set_camera(camera_t cam,  scalar pos[3],scalar dir[3], scalar up[3]){
	scalar *X,*Y,*Z;
	X=cam+CAMERA_X; 	Y=cam+CAMERA_Y; 	Z=cam+CAMERA_Z;
	if(pos){ copy(pos,cam+CAMERA_T);	}
	if(dir || up ){
		if(dir) {COPY(dir,Z,3);}
		if(up) {COPY(up,Y,3);}
		cross(Y,Z,X); 	cross(Z,X,Y);
		normalize(X); 	normalize(Y);	normalize(Z);
	}
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

enum{MESH_CELL_POS=0,MESH_CELL_TEX=3,MESH_CELL_NORMAL=5,MESH_CELL_FULL_SIZE=8};
typedef scalar* mesh_t;

void set_cell(scalar cell[8]){
	glTexCoord2dv(cell+MESH_CELL_TEX);
	glNormal3dv(cell+MESH_CELL_NORMAL);
	glVertex3dv(cell+MESH_CELL_POS);
}

mesh_t create_mesh(uint nrow,uint ncol,uint uclose,uint vclose){
	return ALLOC(((nrow+vclose)*(ncol+uclose))*MESH_CELL_FULL_SIZE);
}

scalar* get_mesh_cell(mesh_t mesh,uint row,uint col,uint nrow,uint ncol,uint uclose,uint vclose){
	return mesh+((row*(ncol+uclose)+col)*MESH_CELL_FULL_SIZE);
}

mesh_t update_mesh(mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose){
	scalar u[3],v[3],*data,du,dv;
	uint i,j;
	/* compute normals */
	
	/* update mesh */
	if(uclose){ data=mesh;	j=ncol*MESH_CELL_FULL_SIZE;	for(i=0;i<nrow;i++){ COPY(data+j,data,MESH_CELL_FULL_SIZE); data+=j+MESH_CELL_FULL_SIZE;}}
	if(vclose){ j=(ncol+uclose)*MESH_CELL_FULL_SIZE; COPY(mesh+nrow*j,mesh,j); }
	/* compute texture coord*/
	nrow+=vclose;	ncol+=uclose;
	du=1.0/(ncol-1.0); 	dv=1.0/(nrow-1.0);
	data=mesh+MESH_CELL_TEX;
	for(i=0;i<nrow;i++){
		for(j=0;j<ncol;j++){
			data[0]=du*j; 	data[1]=dv*i;
			data+=MESH_CELL_FULL_SIZE;
		}
	}
	return mesh;
}

mesh_t draw_mesh(mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose){
	uint i,j,srow;
	scalar *data;
	ncol+=uclose;	srow=ucol*MESH_CELL_FULL_SIZE;	nrow+=vclose; nrow-=1;
	data=mesh;
	for(i=0;i<nrow;i++){
		glBegin(GL_TRIANGLE_STRIP)
		for(j=0;j<ncol;j++){
			set_cell(data); 	set_cell(data+srow);
			data+=MESH_CELL_FULL_SIZE;
		}
		glEnd();
	}
	return mesh;
}
