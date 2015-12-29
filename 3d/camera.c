#include "camera.h"
#include <GL/glew.h>
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
