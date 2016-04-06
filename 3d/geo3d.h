#ifndef GEO3D_H
#define GEO3D_H

#include "types.h"

enum {RAY_SIZE=6,PLANE_SIZE=6,SPHERE_SIZE=4,VECTOR_SIZE=4};

scalar* create_geo3d(uint size);

scalar* diff3d(scalar* curve,scalar* T,uint n,uint closed,uint csize,uint tsize);

#endif