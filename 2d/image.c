#include "image.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../HELPER.h"


static uint WHC[3];

uchar* create_img_data(uint width,uint height){
	WHC[WIDTH]=width; 	WHC[HEIGHT]=height; 	WHC[COMP]=PIXEL_SIZE; 	
	return calloc(height,width*PIXEL_SIZE*sizeof(uchar));
}

void destroy_img_data(uchar* img){	free(img);}

uchar* load_img(char const filepath[]){
	return stbi_load(filepath,WHC+WIDTH,WHC+HEIGHT,WHC+COMP,PIXEL_SIZE);
}

uint get_whc(uint i){ return WHC[i];}

void save_img(char const filepath[],uchar* data,uint W,uint H,uint C){
	stbi_write_png(filepath,W,H,C,data,W*PIXEL_SIZE);
}

uint get_pixel(uint x,uint y,uchar* data,uint W, uint H){
	uchar* pixel=0;
	if(x<W && y<H){ pixel=data+(y*W+x)*PIXEL_SIZE;}
	return pixel?(pixel[0]<<24+pixel[1]<<16+pixel[2]<<8+pixel[3]):0;
}

uchar* set_pixel(uint x,uint y,uint color,uchar* data,uint W, uint H){
	uchar* pixel=0;
	if(x<W && y<H){
		pixel=data+(y*W+x)*PIXEL_SIZE;
		//~ memcpy(pixel,&color,PIXEL_SIZE);
		pixel[3]=color&0xFF; 	color>>=8;
		pixel[2]=color&0xFF;  color>>=8;
		pixel[1]=color&0xFF; color>>=8;
		pixel[0]=color&0xFF; 
	}
	return pixel;
}

uchar* next_pixel(uchar* pixel){	return pixel+PIXEL_SIZE; }

void copy_image_data(uchar* src,uint ds, uchar* dst, uint dd,uint count,uint n){
	while(count--){
		memcpy(dst,src,n);
		src+=ds; 	dst+=dd;
	}
}

uchar* get_block(uint x,uint y,uint w,uint h,uchar* data,uint W, uint H){
	uchar* block;
	w=MIN(W-x,w); 	h=(H-y,h);
	WHC[WIDTH]=w; 	WHC[HEIGHT]=h; 	WHC[COMP]=PIXEL_SIZE;
	block=create_img_data(w,h);
	if(x<W && y<H){ 
		copy_image_data(data+(y*W+x)*PIXEL_SIZE,W*PIXEL_SIZE, block, w*PIXEL_SIZE,h,w*PIXEL_SIZE);
	}
	return block;
}

uchar* set_block(uint x,uint y,uchar* block,uint w,uint h,uchar* data,uint W, uint H){
	uint ww,hh;
	ww=MIN(W-x,w); 	hh=MIN(H-y,h);
	if(x<W && y<H){ 
		copy_image_data(block, w*PIXEL_SIZE,data+(y*W+x)*PIXEL_SIZE,W*PIXEL_SIZE,hh,ww*PIXEL_SIZE);
	}
	return data;
}

void mix(double a,uchar* src,uchar* dst, uint W, uint H){
	uint w;
	double a1,a2;
	while(H--){
		w=W;
		while(w--){
			a1=a*src[3]/255.0;	a2=(1-a)*dst[3]/255.0;
			dst[0]=round(a1*src[0]+a2*dst[0]);
			dst[1]=round(a1*src[1]+a2*dst[1]);
			dst[2]=round(a1*src[2]+a2*dst[2]);
			dst[3]=255;
			src+=PIXEL_SIZE;			dst+=PIXEL_SIZE;
		}
	}
}

/* http://members.chello.at/~easyfilter/bresenham.html */

void draw_line(int x0, int y0, int x1, int y1,uint color,uchar* data, uint W, uint H)
{
   int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
   int err = dx+dy, e2; /* error value e_xy */
   for(;;){  /* loop */
		set_pixel(x0,y0,color,data,W,H);
		if (x0==x1 && y0==y1) break;
		e2 = 2*err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
   }
}
  
void draw_circle(int xm, int ym, int r,uint color,uchar* data, uint W, uint H)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   do {
		set_pixel(xm-x, ym+y,color,data,W,H); /*   I. Quadrant */
		set_pixel(xm-y, ym-x,color,data,W,H); /*  II. Quadrant */
		set_pixel(xm+x, ym-y,color,data,W,H);  /* III. Quadrant */
		set_pixel(xm+y, ym+x,color,data,W,H); /*  IV. Quadrant */
		r = err;
		if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
		if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}
  
void draw_ellipse(int x0, int y0, int x1, int y1,uint color,uchar* data, uint W, uint H)
{
   int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
   long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
   long err = dx+dy+b1*a*a, e2; /* error of 1.step */

   if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
   if (y0 > y1) y0 = y1; /* .. exchange them */
   y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
   a *= 8*a; b1 = 8*b*b;
   do {
		set_pixel(x1, y0,color,data,W,H); /*   I. Quadrant */
		set_pixel(x0, y0,color,data,W,H); /*  II. Quadrant */
		set_pixel(x0, y1,color,data,W,H);  /* III. Quadrant */
		set_pixel(x1, y1,color,data,W,H); /*  IV. Quadrant */
       e2 = 2*err;
       if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */ 
       if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
   } while (x0 <= x1);
   while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
		set_pixel(x0-1, y0,color,data,W,H); /*   I. Quadrant */
		set_pixel(x1+1, y0++,color,data,W,H); /*  II. Quadrant */
		set_pixel(x0-1, y1,color,data,W,H);  /* III. Quadrant */
		set_pixel(x1+1, y1--,color,data,W,H); /*  IV. Quadrant */
   }
}
  
