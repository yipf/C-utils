#include <GL/glew.h>

typedef double scalar;

/* basic matrix functions */

static scalar dot(scalar a[3],scalar b[3]); /* return a.b*/
static scalar* cross(scalar a[3],scalar b[3],scalar c[3]); /* c=axb */
static void normalize(scalar v[3]); /* normalize v */
#define ALLOC(n) calloc(sizeof(scalar),(n))
#define COPY(src,dst,n) memcpy(dst,src,sizeof(scalar)*(n))
#define OP2(A,op,B) 	A[0] op B[0];	A[1] op B[1];	A[2] op B[2];
#define OP3(C,eq,A,op,B) 	C[0] eq A[0] op B[0];	C[1] eq A[1] op B[1];	C[2] eq A[2] op B[2];

static scalar* make_rotate(scalar mat[16],scalar x,scalar y, scalar z, scalar ang);
static scalar* make_scale(scalar mat[16],scalar sx,scalar sy,scalar sz);
static scalar* make_translate(scalar mat[16],scalar tx,scalar ty,scalar tz);
static scalar* invert_mat4x4(scalar mat[16],scalar inv[16]);
static scalar* mult_mat4x4(scalar A[16],scalar B[16],scalar C[16]);

/* functions for camera */

typedef scalar* camera_t;
camera_t create_camera(void);
enum {CAMERAE_Y=1<<2,CAMERA_Z=2<<2,CAMERA_T=3<<2,CAMERA_PROJ=1<<4,CAMERA_VIEW=2<<4,CAMERA_BIAS=3<<4,CAMERA_FULL_SIZE=4<<4};
camera_t set_camera_orient(camera_t cam, scalar to[3], scalar up[3]);
camera_t set_camera_position(camera_t cam, scalar pos[3]);
camera_t set_camera_view(camera_t cam,scalar FOV,scalar near,scalar far);
camera_t set_camera_projection(camera_t cam,scalar proj[4]);
camera_t apply_camera(camera_t cam);

/* draw function for objects */
	
typedef unsigned int uint;

void set_vertex(scalar vertex[8]); /* vertex=< x,y,z,tx,ty,nx,ny,nz > */
void draw_sphere(uint U,uint V);
void draw_plane(scalar size);
void draw_box(void);

enum{MESH_CELL_POS=0,MESH_CELL_TEX=3,MESH_CELL_NORMAL=5,MESH_CELL_FULL_SIZE=8};
typedef scalar* mesh_t;
uint * create_option(uint nrow,uint ncol,uint uclose,uint vclose);
mesh_t create_mesh(uint option[4]); /* option=< nrow,ncol,uclose,vclose > */
mesh_t set_mesh_cell(mesh_t mesh,uint row,uint col,scalar cell[8],uint option[4]);
void compute_normal(mesh_t mesh,uint option[4]);
void compute_texture(mesh_t mesh,uint option[4]);
void draw_mesh(mesh_t mesh,uint option[4]);

/* funtion interface for call-id and matrix stack */

/* shader */