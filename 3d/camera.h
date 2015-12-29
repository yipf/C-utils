#ifndef CAMERA_H
#define CAMERA_H

#include "basic3d.h"

typedef scalar* camera_t;
enum {CAMERAE_Y=1<<2,CAMERA_Z=2<<2,CAMERA_T=3<<2,CAMERA_PROJ=1<<4,CAMERA_VIEW=2<<4,CAMERA_BIAS=3<<4,CAMERA_FULL_SIZE=4<<4};

camera_t create_camera(scalar fov,scalar near,scalar far,scalar wh);
camera_t set_camera(camera_t cam,  scalar pos[3],scalar dir[3], scalar up[3]);
void apply_camera(camera_t cam);

#ENDIF
