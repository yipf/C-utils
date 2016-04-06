#include "mygl3d.h"
#include <stdio.h>

void init_gl_env(void){
	/* initiate glew*/
	GLenum glew_state;
	glew_state=glewInit();
	if(GLEW_OK!=glew_state) {printf("Error Loading GLEW: %d",GLEW_OK);}
	/* initiate opengl environment */
	glClearDepth(1.0);            // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);    // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glFrontFace(GL_CCW);
	glEnable(GL_NORMALIZE);
	glAlphaFunc(GL_GREATER,0.1); 
	glEnable(GL_TEXTURE_2D); // always enable textures
}

void set_viewport(int x,int y,int w, int h){ glViewport(x,y,w,h); }
void set_bg_color(scalar r,scalar g, scalar b, scalar a){ glClearColor (r, g, b, a); }
void clear_buffers(void){ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void begin_draw(int PRIMES){
		switch(PRIMES){
		case POINTS: glBegin(GL_POINTS); break;
		case LINES: glBegin(GL_LINES); break;
		case POLYGON: glBegin(GL_POLYGON); break;
		case TRIANGLES: glBegin(GL_TRIANGLES); break;
		case QUADS: glBegin(GL_QUADS); break;
		case LINE_STRIP: glBegin(GL_LINE_STRIP); break;
		case LINE_LOOP: glBegin(GL_LINE_LOOP); break;
		case TRIANGLE_STRIP: glBegin(GL_TRIANGLE_STRIP); break;
		case TRIANGLE_FAN: glBegin(GL_TRIANGLE_FAN); break;
		case QUAD_STRIP: glBegin(GL_QUAD_STRIP); break;
		default: break;
	}
}

void set_vertex(double x, double y, double z, double u, double v, double nx, double ny, double nz){
	glTexCoord2d(u,v);
	glNormal3d(nx,ny,nz);
	glVertex3d(x,y,z);
}

void end_draw(void){ glEnd(); }

uint begin_call_list(void){
	uint id;
	id = glGenLists (1);
	glNewList(id, GL_COMPILE);
	return id;
}
void end_call_list(void){ glEndList(); }
void call_list(uint id){ glCallList(id);}

void apply_options(int option){
	if(option&TEXTURE_2D){glEnable(GL_TEXTURE_2D);}else{ glDisable(GL_TEXTURE_2D);}
	if(option&LIGHTING){glEnable(GL_LIGHTING);}else{ glDisable(GL_LIGHTING);}
	if(option&CULL_FACE){glEnable(GL_CULL_FACE);}else{ glDisable(GL_CULL_FACE);}
	if(option&BLEND){glEnable(GL_BLEND);}else{ glDisable(GL_BLEND);}
	if(option&FOG){glEnable(GL_FOG);}else{ glDisable(GL_FOG);}
	if(option&FILL){glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);}else{glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);}
	if(option&SMOOTH){glShadeModel(GL_SMOOTH);}else{glShadeModel(GL_FLAT);}
}

int toggle_option(int option,int item){
	return option^item;
}

void push_matrix(scalar* matrix){
	if(matrix){
		glPushMatrix();
		glMultMatrixd(matrix);
	}
}
void pop_matrix(scalar * matrix){ if(matrix){ glPopMatrix();}}

uchar* create_mem_img(uint width, uint height){
	return calloc(width*height,IMAGE_CELL_SIZE*sizeof(uchar));
}

uint mem2texture(uchar* data,uint width, uint height,int linear){
	uint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id );
	glPixelStorei(GL_UNPACK_ALIGNMENT, IMAGE_CELL_SIZE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0,  GL_RGBA, GL_UNSIGNED_BYTE, data );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,linear?GL_LINEAR:GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,linear?GL_LINEAR:GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return id;
}

uint file2texture(char const *filepath){
	uint id;
	uint w,h,comp;
	uchar *data;
	data=stbi_load(filepath,&w,&h,&comp,IMAGE_CELL_SIZE);
	if(!data){
		printf("Error when loading file: %s",filepath);
		return 0;
	}
	id=mem2texture(data,w,h,1);
	free(data);
	return id;
}
void bind_texture(uint id){  glBindTexture(GL_TEXTURE_2D,id);	}

uchar *rgba2color(uchar r,uchar g,uchar b,uchar a){
	uchar* color;
	color=calloc(IMAGE_CELL_SIZE,sizeof(uchar));
	color[0]=r; 	color[1]=g; 	color[2]=b; 	color[3]=a;
	return color;
}

uint color2texture(uchar color[4],int destroy){
	uint id;
	id=mem2texture(color,1,1,0);
	if(destroy){	free(color); }
	return id;
}

uint chess2texture(uint n1,uint n2,uchar color1[4],uchar color2[4],int destroy){
	uint id,i,j,size;
	char * img,*start;
	size=IMAGE_CELL_SIZE*sizeof(uchar);
	img=create_mem_img(n1,n2);
	for(i=0;i<n1;i+=2){
		start=img+i*n2*IMAGE_CELL_SIZE;
		for(j=0;j<n2;j+=2){ memcpy(start+j*IMAGE_CELL_SIZE,color1,size); }
		for(j=1;j<n2;j+=2){ memcpy(start+j*IMAGE_CELL_SIZE,color2,size); }
	}
	for(i=1;i<n1;i+=2){ 
		start=img+i*n2*IMAGE_CELL_SIZE;
		for(j=0;j<n2;j+=2){ memcpy(start+j*IMAGE_CELL_SIZE,color2,size); }
		for(j=1;j<n2;j+=2){ memcpy(start+j*IMAGE_CELL_SIZE,color1,size); }
	}
	id=mem2texture(img,n1,n2,0);
	free(img);
	if(destroy){	free(color1); free(color2); }
	return id;
}

GLhandleARB compile_shader(const char* string,GLenum type){
	GLhandleARB handle;
	GLint result;				// Compilation code result
	GLint errorLoglength;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	handle = glCreateShaderObjectARB(type);
	if (!handle){		printf("Failed creating shader object!");		return 0;	}
	glShaderSourceARB(handle, 1, &string, 0);
	glCompileShaderARB(handle);
	//Compilation checking.
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	if (!result){
		printf("Failed to compile shader:");
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
		errorLogText = malloc(sizeof(char) * errorLoglength);
		glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
		printf("%s\n",errorLogText);
		free(errorLogText);
	}
	return handle;
}

GLhandleARB build_shader(const char* vert,const char* frag){
	GLhandleARB shadowShaderId;
	shadowShaderId = glCreateProgramObjectARB();
	glAttachObjectARB(shadowShaderId,vert?compile_shader(vert,GL_VERTEX_SHADER):0);
	glAttachObjectARB(shadowShaderId,frag?compile_shader(frag,GL_FRAGMENT_SHADER):0);
	glLinkProgramARB(shadowShaderId);
	return shadowShaderId;
}

void apply_shader(GLhandleARB shaderid){	glUseProgramObjectARB(shaderid);}

void create_shadowFBO(GLuint FBO[4],GLuint width,GLuint height){
	FBO[2]=width;	FBO[3]=height;
	/* create a depth texture */
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, FBO+1);
	glBindTexture(GL_TEXTURE_2D,FBO[1]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	/* bind texture to FBO */
	glGenFramebuffersEXT(1, FBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO[0]);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, FBO[1], 0);
}

static GLuint shadowFBO[4];

camera_t create_spotlight(scalar fov,scalar near,scalar far,uint width,uint height){
	create_shadowFBO(shadowFBO,width,height);
	return create_camera(fov,near,far,(width+0.0)/(height+0.0));
}

void set_spotlight(camera_t light,scalar from[3],scalar to[3],scalar up[3]){
	float pos[4];
	pos[0]=from[0]; 	pos[1]=from[1]; 	pos[2]=from[2]; 	pos[3]=1.0;
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	set_camera(light,from,to,up);
}
void render2shadow(camera_t light){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, shadowFBO[0]);
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
	glClear( GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,shadowFBO[2],shadowFBO[3]);
	glCullFace(GL_FRONT); // only draw back faces
	glUseProgramObjectARB(0);
	apply_camera(light);
}
void render2screen(camera_t camera,camera_t light,GLhandleARB shadowid){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glCullFace(GL_BACK);
	glPopAttrib();
	/* transfer the shadowmap to GPU */
	glUseProgramObjectARB(shadowid);
	glActiveTextureARB(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowFBO[1]);
	glUniform1iARB(glGetUniformLocationARB(shadowid,"shadowmap"),  1); 
	glUniform1iARB(glGetUniformLocationARB(shadowid,"tex"),  0); 
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixd(light+CAMERA_BIAS);
	glMultMatrixd(light+CAMERA_PROJ);
	glMultMatrixd(light+CAMERA_VIEW);
	/* prepare to render normally */
	glActiveTextureARB(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	apply_camera(camera);
}

GLuint* get_shadowFBO(void){ return shadowFBO;}
