#ifndef GC_H
#define GC_H

#ifndef GC_PTR_T
#define GC_PTR_T void
#endif

typedef GC_PTR_T * gc_ptr_t;

struct gc_node_;

typedef struct gc_node_{
	gc_ptr_t ptr;
	unsigned int ref;
	struct gc_node_* next;
}gc_node_,*gc_node_t;

static gc_node_t* GC_TABLE;
static unsigned int HASH;

unsigned int ptr2index(gc_ptr_t ptr, unsigned int hash);
static gc_node_t create_gc_node(gc_ptr_t ptr, gc_node_t next);

static gc_ptr_t gc_register(gc_ptr_t ptr);

gc_ptr_t gc_malloc(unsigned int size);
gc_ptr_t gc_calloc(unsigned int n,unsigned int size);

gc_ptr_t gc_ref(gc_ptr_t ptr);

void gc_init(unsigned int hash);

static gc_node_t gc_(gc_node_t head);
void gc(void);

void gc_print(void);

#include <stdlib.h>

static gc_node_t* GC_TABLE=0;

unsigned int ptr2index(gc_ptr_t ptr, unsigned int hash){
	unsigned int n=(long)ptr;
	unsigned int index=n&0xFF;
	index+=(n>>8)&0xFF;
	index+=(n>>16)&0xFF;
	index+=(n>>24)&0xFF;
	return index%hash;
}

gc_node_t create_gc_node(gc_ptr_t ptr, gc_node_t next){
	gc_node_t gnode=malloc(sizeof(gc_node_));
	gnode->ptr=ptr;
	gnode->ref=0;
	gnode->next=next;
	return gnode;
}

void gc_init(unsigned int hash){
	HASH=hash>0?hash:101;
	GC_TABLE=calloc(HASH,sizeof(gc_node_));
}

static gc_ptr_t gc_register(gc_ptr_t ptr){
	unsigned int index;
	index=ptr2index(ptr,HASH);
	GC_TABLE[index]=create_gc_node(ptr,GC_TABLE[index]);
	return ptr;
}

gc_ptr_t gc_malloc(unsigned int size){
	return gc_register(malloc(size));
}

gc_ptr_t gc_calloc(unsigned int n,unsigned int size){
	return gc_register(calloc(n,size));
}

gc_ptr_t gc_ref(gc_ptr_t ptr){
	gc_node_t head;
	head=GC_TABLE[ptr2index(ptr,HASH)];
	while(head&&(head->ptr!=ptr)){head=head->next;}
	if(head){			head->ref++;}
	return ptr;
}

gc_node_t gc_(gc_node_t head){
	gc_node_t pre,next;
	while(head&&head->ref==0){ /* remove the first nodes that with 0 refs */
		next=head->next; 	
		free(head->ptr); 	
		free(head); 
		head=next; 
	}
	pre=head;
	while(pre){
		next=pre->next;
		if(next&&next->ref==0){ /* remove next */
			pre->next=next->next;
			free(next->ptr);
			free(next);
		}else{
			pre->ref--;
			pre=next;
		}
	}
	return head;
}

void gc(void){
	unsigned int index;
	for(index=0;index<HASH;index++){		GC_TABLE[index]=gc_(GC_TABLE[index]);	}
}

#include <stdio.h>
void gc_print(void){
	unsigned int count=0,index;
	gc_node_t node;
	for(index=0;index<HASH;index++){
		node=GC_TABLE[index];
		while(node){
			count++;
			printf("\n%d\t[ref:%d]\t%p",count,node->ref,node->ptr);
			node=node->next;
		}
	}
}

#endif