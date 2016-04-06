#ifndef TENSOR3D_H
#define TENSOR3D_H

#include "types.h"
/* basic matrix functions */

scalar dot(scalar a[3],scalar b[3]); /* return a.b*/
scalar* cross(scalar a[3],scalar b[3],scalar c[3]); /* c=axb */
void normalize(scalar v[3]); /* normalize v */

scalar* make_rotate(scalar mat[16],scalar x,scalar y, scalar z, scalar ang);
scalar* make_scale(scalar mat[16],scalar sx,scalar sy,scalar sz);
scalar* make_translate(scalar mat[16],scalar tx,scalar ty,scalar tz);
scalar* make_projection(scalar mat[16],scalar fov,scalar near, scalar far, scalar wh);
scalar* make_bias(scalar mat[16]);
scalar* make_point_to(scalar mat[16],scalar x,scalar y,scalar z,scalar sx,scalar sy);
scalar* invert_mat4x4(scalar mat[16],scalar inv[16]);
scalar* mult_mat4x4(scalar A[16],scalar B[16],scalar C[16]);

#endif
