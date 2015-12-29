#include "drawers.h"
#include <GL/glew.h>

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
