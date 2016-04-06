#include "camera3d.h"
#include <GL/glew.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* functions for camera */
camera_t create_camera(scalar fov,scalar near,scalar far,scalar wh) { 
	camera_t cam;
	cam=ALLOC(CAMERA_FULL_SIZE);	
	make_translate(cam,0.0,0.0,10.0);	/* initiate the postion and orient */
	make_projection(cam+CAMERA_PROJ,fov,near,far,wh);	/* initiate the proj matrix */
	invert_mat4x4(cam,cam+CAMERA_VIEW);/* initiate the view matrix */
	make_bias(cam+CAMERA_BIAS);/* intiate the bias matrix */
	return cam;
}

camera_t set_camera(camera_t cam,  scalar from[3],scalar to[3], scalar up[3]){
	scalar *X,*Y,*Z;
	X=cam+CAMERA_X; 	Y=cam+CAMERA_Y; 	Z=cam+CAMERA_Z;
	if(from||to || up ){
		if(from){ COPY(from,cam+CAMERA_T,3);	COPY(from,Z,3); 	normalize(Z);}
		if(to) {OP2(Z,-=,to);}
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
	glMultMatrixd(cam+CAMERA_VIEW);
	 /* set modelview matrix*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

camera_t move_camera(camera_t cam,scalar right,scalar up,scalar back){
	scalar *X,*Y,*Z,*T;
	X=cam+CAMERA_X; 	Y=cam+CAMERA_Y; 	Z=cam+CAMERA_Z;	T=cam+CAMERA_T;
	if(right){OP2(T,+=,right*X);}
	if(up){OP2(T,+=,up*Y);}
	if(back){OP2(T,+=,back*Z);}
	invert_mat4x4(cam,cam+CAMERA_VIEW); /* update the view matrix */
	return cam;
}

scalar * xy2dir(camera_t cam,scalar x,scalar y,scalar dir[3]){
	scalar *X,*Y,*Z,*proj;
	dir=dir?dir:ALLOC(3);
	X=cam+CAMERA_X; 	Y=cam+CAMERA_Y;  	Z=cam+CAMERA_Z;  	proj=cam+CAMERA_PROJ; 
	x/=proj[0]; y/=proj[5];
	OP3(dir,=,x*X,+,y*Y);
	OP3(dir,=,dir,-,Z);
	return dir;
}

scalar* xy2ray(camera_t cam,scalar x,scalar y,scalar ray[6]){
	ray=ray?ray:ALLOC(RAY_SIZE);
	COPY(cam+CAMERA_T,ray,3);
	xy2dir(cam,x,y,ray+3);
	return ray;
}

static scalar temp[3];
static scalar Y_AXIS[3]={0,1,0};
static scalar Z_AXIS[3]={0,0,1};

camera_t redirect_camera(camera_t cam,scalar x, scalar y){
	scalar *X,*Y,*Z,*proj;
	X=cam+CAMERA_X; 	Y=cam+CAMERA_Y;  	Z=cam+CAMERA_Z;  	proj=cam+CAMERA_PROJ; 
	x/=proj[0]; y/=proj[5];
	OP3(temp,=,x*X,+,y*Y);
	OP3(temp,=,temp,-,Z);
	if( temp[0]==0.0 && temp[2]==0.0 ) { return cam; }
	cross(temp,Y_AXIS,X);
	OP2(Z,=,-temp)
	cross(Z,X,Y);
	normalize(X); 	normalize(Y);	normalize(Z);
	invert_mat4x4(cam,cam+CAMERA_VIEW); /* update the view matrix */
	return cam;
}

camera_t scale_camera(camera_t cam,scalar sx,scalar sy){
	scalar *proj;
	proj=cam+CAMERA_PROJ; 
	proj[0]*=sx; 	proj[5]*=sy;
	return cam;
}


