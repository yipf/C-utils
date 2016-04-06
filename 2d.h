
#ifndef INTERFACE_IMAGE_H
#define INTERFACE_IMAGE_H

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image_resize.h"

typedef unsigned char uchar;
typedef unsigned int uint;

enum{WIDTH=0,HEIGHT=1,COMP=2,PIXEL_SIZE=4};

uchar* create_img_data(uint width,uint height);
void destroy_img_data(uchar* img);
uchar* load_img(char const filepath[]);
uint get_whc(uint i);

void save_img(char const filepath[],uchar* data,uint W,uint H,uint C);

uint get_pixel(uint x,uint y,uchar* data,uint W, uint H);
uchar* set_pixel(uint x,uint y,uint color,uchar* data,uint W, uint H);
uchar* next_pixel(uchar* pixel);

void copy_image_data(uchar* src,uint ds, uchar* dst, uint dd,uint count,uint n);
uchar* get_block(uint x,uint y,uint w,uint h,uchar* data,uint W, uint H);
uchar* set_block(uint x,uint y,uchar* block,uint w,uint h,uchar* data,uint W, uint H);

void mix(double a,uchar* src,uchar* dst, uint W, uint H);

void draw_line(int x1, int y1, int x2, int y2, uint color,uchar* data, uint W, uint H);
void draw_ellipse(int x, int y, int w, int h, uint color,uchar* data, uint W, uint H);
void draw_circle(int x0, int y0, int radius, uint color,uchar* data, uint W, uint H);


#endif