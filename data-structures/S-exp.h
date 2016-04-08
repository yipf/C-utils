#ifndef S_EXP_H
#define S_EXP_H

enum{NIL=0,LIST,TRUE,FALSE,STRING,NUMBER,INTEGER,SYMBOL,QUOTE,LAMBDA,MACRO,FUNCTION};

struct S_node_;

typedef struct S_node_{
	union {
		struct S_node_ * child;
		int integer;
		double number;
		char* string;
		char* symbol;
		void* ptr;
	}value;
	int type;
	struct S_node_ * next;
}S_node_,*S_node_t;


S_node_t quote_S_node(S_node_t node,unsigned int quote);
S_node_t str2S_node(char* string,int type,unsigned int n);
S_node_t make_S_node(int type);
S_node_t make_S_list(S_node_t head);
S_node_t copy_S_node(S_node_t src,S_node_t dst,int copy_next);

#include <stdio.h>

S_node_t stream2S_node(FILE* stream);
int S_node2stream(S_node_t node,FILE* stream);

#include <stdlib.h>
S_node_t make_S_node(int type){
	S_node_t node;
	node=calloc(1,sizeof(S_node_));
	node->type=type;
	return node;
}

S_node_t make_S_list(S_node_t head){
	S_node_t list;
	list=make_S_node(LIST);
	(list->value).child=head;
	return list;
}


#include <string.h>

S_node_t copy_S_node(S_node_t src,S_node_t dst,int copy_next){
	if((!src)||(src==dst)){ return dst; }
	dst=dst?dst:calloc(1,sizeof(S_node_));
	if(src->type==LIST){
		dst->type=LIST;
		(dst->value).child=copy_S_node((src->value).child,0,1);
	}else{
		memcpy(dst,src,sizeof(S_node_)-sizeof(S_node_t));
	}
	if(copy_next){ dst->next=copy_S_node(src->next,0,1);	}
	return dst;
}

S_node_t str2S_node(char* string,int type,unsigned int n){
	S_node_t node;
	char* str_end;
	node=make_S_node(type);
	if(type==STRING){	
		str_end=calloc(n+1,sizeof(char)); 
		(node->value).string=str_end;		
		while(*(str_end++)=*(string++));	/* copy the string */
		return node;	
	}
	if(strcmp(string,"True")==0){ node->type=TRUE; return node;	}
	if(strcmp(string,"False")==0){ node->type=FALSE; return node;	}
	(node->value).number=strtod(string,&str_end);  /* test if `string' is a number */
	if(*str_end){ /* failed */
		str_end=calloc(n+1,sizeof(char)); 
		(node->value).symbol=str_end;
		while(*(str_end++)=*(string++)); /* copy the string */
		node->type=SYMBOL;
	}else{ /* success */
		node->type=NUMBER; 
	}
	return node;
}

 S_node_t quote_S_node(S_node_t node,unsigned int quote){
	S_node_t head;
	while(quote--){
		head=make_S_node(QUOTE);
		head->next=node;
		node=make_S_list(head);
	}
	return node;
}

#include <stdio.h>
#include <ctype.h>

static char BUFFER[100];

S_node_t stream2S_node(FILE* stream){
	S_node_t head,cur;
	char ch;
	unsigned int  n,inside_string,quote;
	head=make_S_list(0);
	cur=head;
	n=0;	inside_string=0; 	quote=0;
	while((ch=fgetc(stream))!=EOF){
		if(ch=='\\'){ 			BUFFER[n++]=fgetc(stream);			continue;		}
		if(inside_string){
			if(ch=='"'){
				if(n>0){ 	BUFFER[n]='\0'; cur->next=quote_S_node(str2S_node(BUFFER,STRING,n),quote); 	cur=cur->next;	  n=0;	quote=0;}
				inside_string=0;
			}else{				BUFFER[n++]=ch;			}			
			continue;
		}
		if(ch==')'){ break;} /* return list */
		switch(ch){
			case ';':  /* head a commit line */
				if(n>0){ 	BUFFER[n]='\0'; cur->next=quote_S_node(str2S_node(BUFFER,SYMBOL,n),quote);  	cur=cur->next;	  n=0;	quote=0;}
				while((ch=fgetc(stream))!='\n'&&ch!=EOF);
				break;
			case '"': /* head a string */
				if(n>0){ cur->next=quote_S_node(str2S_node(BUFFER,SYMBOL,n),quote);  	cur=cur->next;	  n=0;	quote=0;}
				inside_string=1;
				break;
			case '(':  /* head a list */
				if(n>0){ 	BUFFER[n]='\0'; cur->next=quote_S_node(str2S_node(BUFFER,SYMBOL,n),quote);  	cur=cur->next;	  n=0;	quote=0;}
				cur->next=quote_S_node(stream2S_node(stream),quote);		cur=cur->next;  quote=0;
				break;
			case '`':  /* head a quote */
				if(n>0){ 	BUFFER[n]='\0'; cur->next=quote_S_node(str2S_node(BUFFER,SYMBOL,n),quote);   	cur=cur->next;	  n=0;	quote=0;}
				quote++;
				break;
			default: 
				if(isspace(ch)){ /* if space */
					if(n>0){ 	BUFFER[n]='\0'; cur->next=quote_S_node(str2S_node(BUFFER,SYMBOL,n),quote);  	cur=cur->next;	  n=0;	quote=0;}
				}else{					BUFFER[n++]=ch;				}
				break;
		}
	}
	if(n>0){ 	BUFFER[n]='\0'; cur->next=quote_S_node(str2S_node(BUFFER,SYMBOL,n),quote);  	cur=cur->next;	  n=0;	quote=0;} /*  */
	(head->value).child=head->next; 	head->next=0; 
	return head;
}

int S_node2stream(S_node_t node,FILE* stream){
	S_node_t last;
	if(!node) return 0;
	switch(node->type){
			case NIL:	 			fprintf(stream,"NIL ");											 			break;
			case NUMBER:	 	fprintf(stream,"%lf ",(node->value).number);				break;
			case STRING:			fprintf(stream,"\"%s\" ",(node->value).string);				break;
			case SYMBOL:		fprintf(stream,"%s ",(node->value).string);					break;
			case TRUE:				fprintf(stream,"True ");													break;
			case FALSE:			fprintf(stream,"False ");													break;
			case QUOTE:			fprintf(stream,"quote ");													break;
			case FUNCTION: 	fprintf(stream,"<function:%p> ",(node->value).child); break;
			case MACRO:
			case LAMBDA:
				fprintf(stream,(node->type==MACRO)?"MACRO: ":"LAMBDA: ");	 
			case LIST:				
				fprintf(stream,"( ");	 
				if(node=(node->value).child){ while(node){ S_node2stream(node,stream); node=node->next; }}
				fprintf(stream,") ");		
				break;
		default: 	
				fprintf(stream,"[type:%d] ",node->type);
			break;
	}
	return 0;
}

#endif