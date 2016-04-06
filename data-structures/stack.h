#ifndef STACK_H
#define STACK_H

typedef int stack_value_t;

struct stack_;
typedef struct stack_{
	stack_value_t value;
	struct stack_ * next;
} stack_,* stack_t;

stack_t stack_push(stack_t stack,stack_value_t value);
stack_t stack_pop(stack_t stack);
stack_value_t stack_top(stack_t stack);
stack_t stack_clear(stack_t stack);
stack_t stack_print(stack_t stack,char fmt[]);

#include <stdlib.h>

stack_t stack_push(stack_t stack,stack_value_t value){
	stack_t top;
	top=malloc(sizeof(stack_));
	top->value=value;
	top->next=stack;
	return top;
}

stack_t stack_pop(stack_t stack){
	stack_t next;
	if(stack){
		next=stack->next;
		free(stack);
	}else{
		return stack;
	}
	return next;
}

stack_value_t stack_top(stack_t stack){
	return stack?(stack->value):0;
}

stack_t stack_clear(stack_t stack){
	stack_t next;
	while(stack){
		next=stack->next;
		free(stack);
		stack=next;
	}
	return stack;
}

#include <stdio.h>
stack_t stack_print(stack_t stack,char fmt[]){
	while(stack){
		printf(fmt,stack->value);
		stack=stack->next;
	}
	return stack;
}

#endif