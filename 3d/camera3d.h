#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "tensor3d.h"
#include "geo3d.h"

typedef scalar* camera_t;
enum {CAMERA_X=0,CAMERA_Y=1<<2,CAMERA_Z=2<<2,CAMERA_T=3<<2,CAMERA_PROJ=1<<4,CAMERA_VIEW=2<<4,CAMERA_BIAS=3<<4,CAMERA_FULL_SIZE=4<<4};

camera_t create_camera(scalar fov,scalar near,scalar far,scalar wh);
camera_t set_camera(camera_t cam,  scalar pos[3],scalar dir[3], scalar up[3]);
void apply_camera(camera_t cam);
	
camera_t move_camera(camera_t cam,scalar right,scalar up,scalar back);

scalar * xy2dir(camera_t cam,scalar x,scalar y,scalar dir[3]);
scalar* xy2ray(camera_t cam,scalar x,scalar y,scalar ray[6]);
camera_t redirect_camera(camera_t cam,scalar x, scalar y);
camera_t scale_camera(camera_t cam,scalar sx,scalar sy);
	
#endif
