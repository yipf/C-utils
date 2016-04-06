#ifndef MYGL_H
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
#endif