#ifndef SHAPES3D_H
#define SHAPES3D_H

#include "types.h"

enum{MESH_CELL_POS=0,MESH_CELL_TEX=3,MESH_CELL_NORMAL=5,MESH_CELL_FULL_SIZE=8};
typedef scalar* mesh_t;

void set_cell(scalar cell[8]);
mesh_t create_mesh(uint nrow,uint ncol,uint uclose,uint vclose);
scalar* get_mesh_cell(uint row,uint col,mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose);
mesh_t update_mesh(mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose);
mesh_t draw_mesh(mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose);

typedef scalar* curve_t;
typedef scalar* path_t;

enum{CURVE_CELL_SIZE=4};
curve_t create_curve(uint n,int closed);
scalar* get_curve_cell(curve_t curve,uint n);
curve_t draw_curve(curve_t curve,uint n, int closed);

enum{PATH_X=0,PATH_Y=4,PATH_Z=8,PATH_T=12,PATH_CELL_SIZE=16};
path_t curve2path(path_t path,curve_t curve,uint n,uint closed);
scalar* get_path_cell(path_t path,uint n);
mesh_t create_loft(mesh_t mesh,path_t path,curve_t curve,uint np,uint pclosed,uint nc,uint cclosed);

void draw_plane(void);
void draw_cubic(void);
void splite_triangle(scalar A[3],scalar B[3],scalar C[3],uint level);
void draw_sphere(uint level);
void draw_sphere_uv(uint U,uint V);

#endif
