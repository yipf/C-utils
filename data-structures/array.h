#ifndef ARRAY_H
#define ARRAY_H

#ifndef ARRAY_VALUE_T
#define ARRAY_VALUE_T int
#endif

typedef ARRAY_VALUE_T array_value_t;

typedef unsigned int array_size_t;
typedef struct array_{
	array_value_t* data;
	array_size_t n,size;
}array_,*array_t;

static array_value_t* array_resize_data(array_value_t* src,array_size_t n_src,array_size_t n_dst,int destroy_src);
array_t array_create(array_size_t size);
array_t array_resize(array_t array, array_size_t size);
array_t array_destroy(array_t array);
array_t array_insert(array_t array,array_size_t pos,array_value_t value);
array_t array_remove(array_t array,array_size_t start,array_size_t end);
array_t array_clear(array_t array);
array_t array_print(array_t array,char fmt[]);

#include <stdlib.h>
#include <string.h>


array_t array_create(array_size_t size){
	array_t array;
	size=size?size:10;
	array=malloc(sizeof(array_));
	array->data=calloc(size,sizeof(array_value_t));
	array->n=0;
	array->size=size;
	return array;
}

array_value_t* array_resize_data(array_value_t* src,array_size_t n_src,array_size_t n_dst,int destroy_src){
	array_value_t* dst;
	dst=calloc(n_dst,sizeof(array_value_t));
	memcpy(dst,src,(n_src<n_dst?n_src:n_dst)*sizeof(array_value_t));
	if(destroy_src){ free(src);	}
	return dst;
}

array_t array_resize(array_t array, array_size_t size){
	size=size?size:10;
	if(array){
		array->data=array_resize_data(array->data,array->size,size,1);
		array->size=size;
		array->n=(array->n)<size?(array->n):size;
	}else{
		array=array_create(size);
	}
	return array;
}

array_t array_destroy(array_t array){
	if(array){
		free(array->data);
		free(array);
	}
	return array;
}

array_t array_insert(array_t array,array_size_t pos,array_value_t value){
	array_size_t size,n;
	array_value_t* data;
	if(array){
		n=array->n;
		size=array->size;
		if(n>=size){ array=array_resize(array,size<<1); }
		data=array->data;
		pos=pos<n?pos:n;
		while(n>pos){data[n]=data[n-1]; n--; }
		data[pos]=value;
		array->n++;
	}
	return array;
}

array_t array_remove(array_t array,array_size_t start,array_size_t end){
	array_size_t n;
	array_value_t* data;
	if(array&&(data=array->data)&&(n=array->n)>0){
		end=end<n?end:(n-1);
		start=start<n?start:(n-1);
		end=end<start?start:end;
		if(start>=n){
			array->n--;
		}else{
			end=end-start+1;
			n=n-end;
			while(start<n){data[start]=data[start+end]; start++;}
			array->n=n;
		}
	}
	return array;
}

array_t array_clear(array_t array){
	if(array){
		memset(array->data,0,(array->n)*sizeof(array_value_t));
	}
	return array;
}

#include <stdio.h>
array_t array_print(array_t array,char fmt[]){
	array_size_t i,n;
	array_value_t* data;
	n=array->n;
	data=array->data;
	for(i=0;i<n;i++){
		printf("%2d/%-8d",i,n);printf(fmt,data[i]);
	}
	return array;
}

#endif