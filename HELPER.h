
/* MACROS for debug */

#include <stdio.h>

#define MSG(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);
#define EXCEPTION(condition,value,msg) if(condition){printf("\n%s:%d::\t%s",__FILE__,__LINE__,msg); return value;}

#define MAX(a,b) (a)>(b)?(a):(b)
#define MIN(a,b) (a)<(b)?(a):(b)
#define ABS(a) ((a)>=0)?(a):(-(a))