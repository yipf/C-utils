#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#ifndef HASH_TABLE_VALUE_T
#define HASH_TABLE_VALUE_T char*
#endif

typedef HASH_TABLE_VALUE_T hash_table_value_t;
typedef char* hash_table_key_t;

struct hash_table_node_;

typedef struct hash_table_node_{
	hash_table_key_t key;
	hash_table_value_t value;
	struct hash_table_node_ * next;
}hash_table_node_,*hash_table_node_t;

typedef struct hash_table_{
	hash_table_node_t* table;
	unsigned int hash;
}hash_table_,*hash_table_t;

hash_table_t hash_table_create(unsigned int hash);
static unsigned int hash_table_key2index(hash_table_key_t key,unsigned int hash);
hash_table_t hash_table_set(hash_table_t ht,hash_table_key_t key,hash_table_value_t value);
hash_table_t hash_table_unset(hash_table_t ht,hash_table_key_t key);
hash_table_value_t hash_table_get(hash_table_t ht,hash_table_key_t key);

#include <stdio.h>
hash_table_t hash_table2stream(hash_table_t ht,FILE* stream,char* value_fmt);

#include <stdlib.h>
hash_table_t hash_table_create(unsigned int hash){
	hash_table_t ht;
	ht=malloc(sizeof(hash_table_));
	hash=hash>0?hash:11;
	ht->table=calloc(hash,sizeof(hash_table_node_t));
	ht->hash=hash;
	return ht;
}

unsigned int hash_table_key2index(hash_table_key_t key,unsigned int hash){
	unsigned int index=0,n;
	while(n=*(key++)){
		index+=n; 
		while(index>=hash){ index-=hash; }
	}
	return index;
}

#include <string.h>
hash_table_t hash_table_set(hash_table_t ht,hash_table_key_t key,hash_table_value_t value){
	unsigned int index;	
	hash_table_node_t head;
	index=hash_table_key2index(key,ht->hash);
	head=ht->table[index];
	while(head&&strcmp(key,head->key)){ head=head->next;}
	if(!head){
		head=malloc(sizeof(hash_table_node_));
		head->key=key;
		head->value=value;
		head->next=ht->table[index];
		ht->table[index]=head;
	}
	head->value=value;
	return ht;
}

hash_table_t hash_table_unset(hash_table_t ht,hash_table_key_t key){
	unsigned int index;	
	hash_table_node_t head,node;
	index=hash_table_key2index(key,ht->hash);
	head=ht->table[index];
	if(head){
		if(strcmp(head->key,key)==0){
			ht->table[index]=head->next;
			free(head);
		}else{
			while((node=head->next)&&strcmp(node->key,key)){head=node;}
			if(node){	head->next=node->next; 	free(node);	}
		}
	}
	return ht;
}

hash_table_value_t hash_table_get(hash_table_t ht,hash_table_key_t key){
	hash_table_node_t head;
	head=ht->table[hash_table_key2index(key,ht->hash)];
	while(head&&strcmp(key,head->key)){ head=head->next;}
	return head?head->value:0;
}

hash_table_t hash_table2stream(hash_table_t ht,FILE* stream,char* value_fmt){
	unsigned int index,count;	
	hash_table_node_t *table,head;
	table=ht->table;
	for(index=0,count=0;index<(ht->hash);index++){
		head=table[index];
		while(head){
			count++;
			fprintf(stream,"\n%d[%s]:\t",count,head->key);fprintf(stream,value_fmt,head->value);
			head=head->next;
		}
	}
	return ht;
}

#endif