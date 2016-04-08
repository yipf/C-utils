#include "../data-structures/S-exp.h"

#define HASH_STACK_VALUE_T S_node_t
#include "../data-structures/hash-stack.h"

typedef S_node_t (*func_t)(S_node_t args);

S_node_t add(S_node_t args){
	S_node_t node;
	double number=0.0;
	node=make_S_node(NUMBER);
	while(args){
		if(args->type==NUMBER);
		number+=(args->value).number;
		args=args->next;
	}
	(node->value).number=number;
	return node;
}

S_node_t lambda(S_node_t args){
	S_node_t node;
	node=make_S_node(LAMBDA);
	(node->value).child=args;
	return node;
}

S_node_t macro(S_node_t args){
	S_node_t node;
	node=make_S_node(MACRO);
	(node->value).child=args;
	return node;
}

hash_stack_t ENV=0;

S_node_t set(S_node_t args){
	S_node_t val=args->next;
	hash_stack_push(ENV,(args->value).symbol,val);
	return val;
}

S_node_t make_func_node(func_t function){
	S_node_t fnode=make_S_node(FUNCTION);
	(fnode->value).ptr=function;
	return fnode;
}

S_node_t eval(S_node_t node);

S_node_t get_child(S_node_t node){return node?(node->value).child:0; }

S_node_t apply(S_node_t lambda,S_node_t vals){
	S_node_t args,body;
	args=get_child(lambda);
	if(args){
		body=args->next;
		args=get_child(args);
		/* push values*/
		while(args&&vals){	
			hash_stack_push(ENV,(args->value).symbol,vals);
			args=args->next; 	vals=vals->next;
		}
		vals=args;
		/* process body*/
		body=copy_S_node(body,0,0);
		body=eval(body);
		/* pop values*/
		args=get_child(get_child(lambda));
		while(args!=vals){ 	
			hash_stack_pop(ENV,(args->value).symbol);
			args=args->next;
		}
		return body;
	}
	return 0;
}

S_node_t eval(S_node_t node){
	S_node_t res=0;
	switch(node->type){
		case SYMBOL:
			res=hash_stack_get(ENV,(node->value).symbol);
			break;
		case LIST:
			/* eval every elements */
			res=(node->value).child;
			if(!res) break; /* if node is an empty list then do nothing */
			res=eval(res);
			switch(res->type){
				case QUOTE:
					res=res->next; 
					break;
				case FUNCTION:
					while(res=res->next){res=eval(res);}
					res=(node->value).child;
					res=((func_t)(res->value).ptr)(res->next);
					break;
				case LAMBDA:
					while(res=res->next){res=eval(res);}
				case MACRO:
					res=(node->value).child;
					res=apply(res,res->next);
					/* apply function*/
					break;
				default: 
					while(res=res->next){res=eval(res);}
					res=0;
					break;
			}
		default: 
			break;
	}
	return res?copy_S_node(res,node,0):node;
}

hash_stack_t init_env(hash_stack_t env){
	hash_stack_push(env,"+",make_func_node(add));
	hash_stack_push(env,"eval",make_func_node(eval));
	hash_stack_push(env,"macro",make_func_node(macro));
	hash_stack_push(env,"lambda",make_func_node(lambda));
	hash_stack_push(env,"set",make_func_node(set));
	return env;
}

int main(void){
	S_node_t node;
	ENV=hash_stack_create(101);
	ENV=init_env(ENV);
	node=stream2S_node(stdin);
	S_node2stream(node,stdout);
	printf("\n=\n");
	eval(node);
	S_node2stream(node,stdout);
	printf("\n");
	return 0;
}