#ifndef GEO3D_H
#define GEO3D_H

#include "types3d.h"

enum {RAY_SIZE=6};
typedef scalar* ray_t;

scalar* diff3d(scalar* curve,scalar* T,uint n,uint closed,uint csize,uint tsize);

#endif