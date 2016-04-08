//~ #include "stack.h"
//~ int main(void){
	//~ stack_t stack=0;
	//~ stack=stack_push(stack,1);
	//~ stack=stack_push(stack,2);
	//~ stack=stack_push(stack,3);
	//~ stack=stack_push(stack,4);
	//~ printf("top:%d\n",stack_top(stack));
	//~ stack=stack_pop(stack);
	//~ printf("pop:%d\n",stack_top(stack));
	//~ stack=stack_push(stack,5);
	//~ stack_print(stack,"%d\n");
	//~ stack=stack_clear(stack);
	//~ printf("%p\n",stack);
	//~ return 0;
//~ }

//~ #include "array.h"
//~ int main(void){
	//~ array_t array=array_create(2);
	//~ array_insert(array,0,1);
	//~ array_insert(array,0,2);
	//~ array_insert(array,0,3);
	//~ array_insert(array,0,4);
	//~ printf("before:\n");
	//~ array_print(array,"%d\n");
	//~ array_remove(array,0,0);
	//~ printf("after:\n");array_print(array,"%d\n");
	//~ return 0;
//~ }

//~ #include "S-exp.h"
//~ int main(void){
	//~ S_node_t s;
	//~ s=stream2S_node(stdin);
	//~ S_node2stream(s,stdout);
	//~ return 0;
//~ }

//~ #include "S-exp.h"
//~ int main(void){
	//~ S_node_t s;
	//~ s=stream2S_node(stdin);
	//~ S_node2stream(s,stdout);
	//~ return 0;
//~ }

//~ #include "hash-table.h"
//~ int main(void){
	//~ hash_table_t ht=hash_table_create(17);
	//~ hash_table_set(ht,"a","A");
	//~ hash_table_set(ht,"b","B");
	//~ hash_table_set(ht,"c","C");
	//~ hash_table_set(ht,"d","D");
	//~ hash_table2stream(ht,stdout,"%s");
	//~ hash_table_unset(ht,"c");
	//~ hash_table2stream(ht,stdout,"%s");
	//~ printf("\n%s\n",hash_table_get(ht,"a"));
	//~ return 0;
//~ }

//~ #include "../data-structures/hash-stack.h"
//~ int main(void){
	//~ hash_stack_t ht=hash_stack_create(131);
	//~ hash_stack_push(ht,"aa","A");
	//~ hash_stack_push(ht,"b","B");
	//~ hash_stack_push(ht,"c","C");
	//~ hash_stack_push(ht,"d","D");
	//~ hash_stack_push(ht,"d","B");
	//~ hash_stack_push(ht,"d","A");
	//~ hash_stack2stream(ht,stdout,"%s");
	//~ hash_stack_pop(ht,"d");
	//~ hash_stack_pop(ht,"d");
	//~ hash_stack_pop(ht,"d");
	//~ hash_stack2stream(ht,stdout,"%s");
	//~ printf("\n%s\n",hash_stack_get(ht,"aa"));
	//~ return 0;
//~ }


#include "../data-structures/gc.h"

void* f(unsigned int n){
	return gc_ref(malloc(n));
}

int main(void){
	void * p;
	int i,j;
	gc_init(101);
	p=f(100000);
	gc_ref(p);
	printf("\nref1");gc_print();
	gc_ref(p);
	printf("\nref2");gc_print();
	gc_ref(p);
	p=f(100000);
	p=f(100000);
	printf("\nref3");gc_print();
	p=f(100000);
	p=f(100000);
	p=f(100000);
	printf("\nBefore");gc_print();
	gc();
	printf("\nafter");gc_print();
	printf("\n");
	return 0;
}

