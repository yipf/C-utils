
typedef double scalar;
typedef unsigned int uint;

#define ALLOC(n) calloc(sizeof(scalar),(n))
#define COPY(src,dst,n) memcpy(dst,src,sizeof(scalar)*(n))
#define OP2(A,op,B) 	A[0] op B[0];	A[1] op B[1];	A[2] op B[2];
#define OP3(C,eq,A,op,B) 	C[0] eq A[0] op B[0];	C[1] eq A[1] op B[1];	C[2] eq A[2] op B[2];typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

typedef unsigned int GLhandleARB;

typedef 	int GLenum;#ifndef CAMERA3D_H
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
#ifndef GEO3D_H
#define GEO3D_H

#include "types.h"

enum {RAY_SIZE=6,PLANE_SIZE=6,SPHERE_SIZE=4,VECTOR_SIZE=4};

scalar* create_geo3d(uint size);

scalar* diff3d(scalar* curve,scalar* T,uint n,uint closed,uint csize,uint tsize);

#endif#ifndef MYGL_H
#define MYGL_H

#include "types.h"

#include <GL/glew.h>

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image_resize.h"

#include "camera3d.h"

void init_gl_env(void);
void set_viewport(int x,int y,int w, int h);
void set_bg_color(scalar r,scalar g, scalar b, scalar a);
void clear_buffers(void);

enum{POINTS,LINES,POLYGON,TRIANGLES,QUADS,LINE_STRIP,LINE_LOOP,TRIANGLE_STRIP,TRIANGLE_FAN,QUAD_STRIP};
void begin_draw(int PRIMES);
void set_vertex(double x, double y, double z, double u, double v, double nx, double ny, double nz);
void end_draw(void);

uint begin_call_list(void);
void end_call_list(void);
void call_list(uint id);

enum{TEXTURE_2D=2,LIGHTING=4,CULL_FACE=8,BLEND=16,FILL=32,SMOOTH=64,FOG=128};
void apply_options(int option);
int toggle_option(int option,int item);

void push_matrix(double* matrix);
void pop_matrix(double * matrix);

typedef unsigned char uchar;

enum {IMAGE_CELL_SIZE=4};
uchar* create_mem_img(uint width, uint height);
uint mem2texture(uchar* data,unsigned int width, unsigned int height,int linear);
uint file2texture(char const *filepath);
void bind_texture(uint id);

uchar *rgba2color(uchar r,uchar g,uchar b,uchar a);
uint color2texture(uchar color[4],int destroy);
uint chess2texture(uint n1,uint n2,uchar color1[4],uchar color2[4],int destroy);


GLhandleARB compile_shader(const char* string,GLenum type);
GLhandleARB build_shader(const char* vert,const char* frag);
void apply_shader(GLhandleARB shaderid);
void create_shadowFBO(GLuint FBO[4],GLuint width,GLuint height);
	
camera_t create_spotlight(scalar fov,scalar near,scalar far,uint weight,uint height);
void set_spotlight(camera_t light,scalar from[3],scalar to[3],scalar up[3]);
void render2shadow(camera_t light);
void render2screen(camera_t camera,camera_t light,GLhandleARB shadowid);

GLuint* get_shadowFBO(void);
#endif#ifndef SHAPES3D_H
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
