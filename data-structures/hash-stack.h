#ifndef HASH_STACK_H
#define HASH_STACK_H

/* treat the element of hash table as a stack,which can be used to implement local variables or up-values*/

typedef char* hash_stack_value_t;
typedef char* hash_stack_key_t;

struct hash_stack_value_node_;

typedef struct hash_stack_value_node_{
	hash_stack_value_t value;
	struct hash_stack_value_node_ * next;
}hash_stack_value_node_,*hash_stack_value_node_t;

struct hash_stack_node_;

typedef struct hash_stack_node_{
	hash_stack_key_t key;
	hash_stack_value_node_t vnode;
	struct hash_stack_node_ * next;
}hash_stack_node_,*hash_stack_node_t;

typedef struct hash_stack_{
	hash_stack_node_t* stack;
	unsigned int hash;
}hash_stack_,*hash_stack_t;

hash_stack_t hash_stack_create(unsigned int hash);
static unsigned int hash_stack_key2index(hash_stack_key_t key,unsigned int hash);
static hash_stack_value_node_t push_value(hash_stack_value_node_t top,hash_stack_value_t value);
static hash_stack_value_node_t pop_value(hash_stack_value_node_t top);
hash_stack_t hash_stack_push(hash_stack_t ht,hash_stack_key_t key,hash_stack_value_t value);
hash_stack_t hash_stack_pop(hash_stack_t ht,hash_stack_key_t key);
hash_stack_value_t hash_stack_get(hash_stack_t ht,hash_stack_key_t key);

#include <stdio.h>
hash_stack_t hash_stack2stream(hash_stack_t ht,FILE* stream,char* value_fmt);

#include <stdlib.h>
hash_stack_t hash_stack_create(unsigned int hash){
	hash_stack_t ht;
	ht=malloc(sizeof(hash_stack_));
	hash=hash>0?hash:11;
	ht->stack=calloc(hash,sizeof(hash_stack_node_t));
	ht->hash=hash;
	return ht;
}

unsigned int hash_stack_key2index(hash_stack_key_t key,unsigned int hash){
	unsigned int index,n;
	while(n=*(key++)){
		index+=n; 
		while(index>=hash){ index-=hash; }
	}
	return index;
}

hash_stack_value_node_t push_value(hash_stack_value_node_t top,hash_stack_value_t value){
	hash_stack_value_node_t vnode;
	vnode=malloc(sizeof(hash_stack_value_node_));
	vnode->value=value;
	vnode->next=top;
	return vnode;
}

#include <string.h>
hash_stack_t hash_stack_push(hash_stack_t ht,hash_stack_key_t key,hash_stack_value_t value){
	unsigned int index;	
	hash_stack_node_t head;
	index=hash_stack_key2index(key,ht->hash);
	head=ht->stack[index];
	while(head&&strcmp(key,head->key)){ head=head->next;}
	if(!head){
		head=malloc(sizeof(hash_stack_node_));
		head->key=key;
		head->vnode=0;
		head->next=ht->stack[index];
		ht->stack[index]=head;
	}
	head->vnode=push_value(head->vnode,value);
	return ht;
}

hash_stack_value_node_t pop_value(hash_stack_value_node_t top){
	hash_stack_value_node_t next;
	next=top;
	if(top){ next=top->next; free(top); }
	return next;
}

hash_stack_t hash_stack_pop(hash_stack_t ht,hash_stack_key_t key){
	unsigned int index;	
	hash_stack_node_t head,node;
	hash_stack_value_node_t vnode;
	index=hash_stack_key2index(key,ht->hash);
	head=ht->stack[index];
	if(head){
		if(strcmp(head->key,key)==0){ /* if head is the target stack */
			vnode=pop_value(head->vnode);
			if(vnode){
				head->vnode=vnode;
			}else{ /* the head is empty node, free it*/
				ht->stack[index]=head->next;
				free(head);
			}
		}else{/* else if node is the target stack */
			while((node=head->next)&&strcmp(node->key,key)){head=node;}
			vnode=pop_value(node->vnode);
			if(vnode){
				node->vnode=vnode;
			}else{ /* the head is empty node, free it*/
				head->next=node->next;
				free(node);
			}
		}
	}
	return ht;
}

hash_stack_value_t hash_stack_get(hash_stack_t ht,hash_stack_key_t key){
	hash_stack_node_t head;
	head=ht->stack[hash_stack_key2index(key,ht->hash)];
	while(head&&strcmp(key,head->key)){ head=head->next;}
	return head?((head->vnode)->value):0;
}

hash_stack_t hash_stack2stream(hash_stack_t ht,FILE* stream,char* value_fmt){
	unsigned int index,count;	
	hash_stack_node_t *stack,head;
	stack=ht->stack;
	for(index=0,count=0;index<(ht->hash);index++){
		head=stack[index];
		while(head){
			count++;
			fprintf(stream,"\n%d[%s]:\t",count,head->key);fprintf(stream,value_fmt,(head->vnode)->value);
			head=head->next;
		}
	}
	return ht;
}

#endif