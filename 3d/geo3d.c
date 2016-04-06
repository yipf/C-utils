
#include "geo3d.h"

#include <stdlib.h>

scalar* diff3d(scalar* curve,scalar* T,uint n,uint closed,uint csize,uint tsize){
	scalar *data,*prev,*next;
	uint i;
	T=T?T:ALLOC(n*tsize);
	data=T;
	for(i=0;i<n;i++){
		prev=(i==0)?(closed?(curve+(n-1)*csize):curve):(curve-csize);
		next=(i==(n-1))?(closed?(curve-i*csize):curve):(curve+csize);
		OP3(data,=,next,-,prev)
		data+=tsize; 	curve+=csize;
	}
	return T;
}

scalar* create_geo3d(uint size){ return ALLOC(size);}