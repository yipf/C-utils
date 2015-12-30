#include "shapes3d.h"
#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#include "tensor3d.h"
#include "geo3d.h"

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
	scalar u[3],v[3],*data,*prev,*next,du,dv;
	uint i,j,row_size;
	row_size=(ncol+uclose)*MESH_CELL_FULL_SIZE;
	/* compute normals */
	for(i=0;i<nrow;i++){
		data=mesh+i*row_size+MESH_CELL_NORMAL;
		for(j=0;j<ncol;j++){
			prev=(j==0)?(uclose?(data+(ncol-1)*MESH_CELL_FULL_SIZE):data):(data-MESH_CELL_FULL_SIZE);	
			next=(j==(ncol-1))?(uclose?(data-j*MESH_CELL_FULL_SIZE):data):(data+MESH_CELL_FULL_SIZE);
			OP3(u,=,next,-,prev)
			prev=(i==0)?(vclose?(data+(nrow-1)*row_size):data):(data-row_size);
			next=(i==(nrow-1))?(vclose?(data-i*row_size):data):(data+row_size);
			OP3(v,=,next,-,prev)
			cross(u,v,data); 	normalize(data);
			data+=MESH_CELL_FULL_SIZE;
		}
	}
	/* update mesh */
	if(uclose){ data=mesh;	j=ncol*MESH_CELL_FULL_SIZE;	for(i=0;i<nrow;i++){ COPY(data,data+j,MESH_CELL_FULL_SIZE); data+=row_size;}}
	if(vclose){ j=(ncol+uclose)*MESH_CELL_FULL_SIZE; COPY(mesh,mesh+nrow*j,j); }
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
	uint i,j,row_size;
	scalar *data;
	ncol+=uclose;	row_size=ncol*MESH_CELL_FULL_SIZE;	nrow+=vclose; nrow-=1;
	data=mesh;
	for(i=0;i<nrow;i++){
		glBegin(GL_TRIANGLE_STRIP);
		for(j=0;j<ncol;j++){
			set_cell(data+row_size); set_cell(data); 	
			data+=MESH_CELL_FULL_SIZE;
		}
		glEnd();
	}
	return mesh;
}

curve_t create_curve(uint n,uint closed){
	return ALLOC(n*CURVE_CELL_SIZE);
}

curve_t draw_curve(curve_t curve,uint n,uint closed){
	uint i;
	glBegin(GL_LINE_STRIP);
	for(i=0;i<n;i++){
		glVertex3dv(curve);
		curve+=CURVE_CELL_SIZE;
	}
	curve-=(n-1)*CURVE_CELL_SIZE;
	if(closed){glVertex3dv(curve);}
	glEnd();
	return curve;
}

path_t curve2path(curve_t curve,uint n,uint closed){
	scalar *X,*Y,*Z,*T;
	path=path?path:ALLOC(n*PATH_CELL_SIZE);
	X=path+PATH_X; 	Y=path+PATH_Y;		Z=path+PATH_Z;	T=path+PATH_T;
	diff3d(curve,Z,n,closed,CURVE_CELL_SIZE,PATH_CELL_SIZE);
	diff3d(Z,Y,n,closed,PATH_CELL_SIZE,PATH_CELL_SIZE);
	while(n--){
		COPY(curve,T,3);
		cross(Y,Z,X); 	cross(Z,X,Y);
		normalize(X); 	normalize(Y); 	normalize(Z);
		X+=PATH_CELL_SIZE;	Y+=PATH_CELL_SIZE; 	Z+=PATH_CELL_SIZE; 	T+=PATH_CELL_SIZE; 	curve+=CURVE_CELL_SIZE;
	}
	return path;
}

mesh_t create_loft(path_t path,curve_t curve,uint np,uint pclosed,uint nc,uint cclosed){
	mesh_t mesh;
	scalar  *X,*Y,*Z,*T,*cp,*row,row_size;
	uint i,j;
	mesh=create_mesh(np,nc,cclosed,pclosed);
	row_size=(nc+cclosed)*MESH_CELL_FULL_SIZE;
	X=path+PATH_X; 	Y=path+PATH_Y;		Z=path+PATH_Z;	T=path+PATH_T;
	for(i=0;i<np;i++){
		row=mesh+i*row_size;
		cp=curve;
		for(j=0;j<nc;j++){
			x=cp[0]; 	y=cp[1]; 	z=cp[2];
			OP3(row,=,T,+,x*X)
			OP3(row,+=,y*Y,+,z*Z)
			row+=MESH_CELL_FULL_SIZE;	cp+=CURVE_CELL_SIZE;
		}
		X+=PATH_CELL_SIZE; 	Y+=PATH_CELL_SIZE;	Z+=PATH_CELL_SIZE; T+=PATH_CELL_SIZE;
	}
	return update_mesh(mesh,np,nc,cclosed,pclosed);
}


void draw_plane(void){
	glBegin(GL_QUADS);
	glNormal3f(0.0,0.0,1.0);
	glTexCoord2f(0.0f,0.0f); 	glVertex3f(-1.0f,-1.0f,0.0);
	glTexCoord2f(1.0f,0.0f); 	glVertex3f(1.0f,-1.0f,0.0);
	glTexCoord2f(1.0f,1.0f); 	glVertex3f(1.0f,1.0f,0.0);
	glTexCoord2f(0.0f,1.0f); 	glVertex3f(-1.0f,1.0f,0.0);
	glEnd();
}

void draw_cubic(void){
	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f); // 前面
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的左上
		glNormal3f(0.0,0.0,-1.0);// 后面
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// 纹理和四边形的左下
		glNormal3f(0.0f,1.0f,0.0f);// 顶面
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// 纹理和四边形的右上
		glNormal3f(0.0f,-1.0f,0.0f);// 底面
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
		glNormal3f(1.0f,0.0f,0.0f);// 右面
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
		glNormal3f(-1.0f,0.0f,0.0f);// 左面
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// 纹理和四边形的左上
	glEnd();
}

void splite_triangle(scalar A[3],scalar B[3],scalar C[3],uint level){
	scalar M[3];
	uint i;
	if(level--){
		for(i=0;i<3;i++){	M[i]=A[i]+B[i]+C[i];	}
		normalize(M); 
		splite_triangle(M,A,B,level);		
		splite_triangle(M,B,C,level);		
		splite_triangle(M,C,A,level);
	}else{
		glNormal3dv(A); glVertex3dv(A);
		glNormal3dv(B); glVertex3dv(B);
		glNormal3dv(C); glVertex3dv(C);
	}
}

static scalar P[4][3]={
{-0.5773502691896,0.5773502691896,0.5773502691896},
{0.5773502691896,0.5773502691896,-0.5773502691896},
{-0.5773502691896,-0.5773502691896,-0.5773502691896},
{0.5773502691896,-0.5773502691896,0.5773502691896}};

void draw_sphere(uint level){
	glBegin(GL_TRIANGLES);
	splite_triangle(P[0],P[1],P[2],level);
	splite_triangle(P[1],P[3],P[2],level);
	splite_triangle(P[1],P[0],P[3],level);
	splite_triangle(P[0],P[2],P[3],level);
	glEnd();
}

