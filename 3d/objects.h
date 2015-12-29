#ifndef CAMERA_H
#define CAMERA_H

#include "basic3d.h"

enum{MESH_CELL_POS=0,MESH_CELL_TEX=3,MESH_CELL_NORMAL=5,MESH_CELL_FULL_SIZE=8};
typedef scalar* mesh_t;
typedef unsigned int uint;

void set_cell(scalar cell[8]);
mesh_t create_mesh(uint nrow,uint ncol,uint uclose,uint vclose);
scalar* get_mesh_cell(mesh_t mesh,uint row,uint col,uint nrow,uint ncol,uint uclose,uint vclose);
mesh_t update_mesh(mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose);
mesh_t draw_mesh(mesh_t mesh,uint nrow,uint ncol,uint uclose,uint vclose);

#ENDIF
