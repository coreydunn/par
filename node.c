#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"node.h"

Node node_new(void)
{
	Node n={
		.str=str_new(),
		.nodes=vec_new(sizeof(Node)), // Empty Vec
	};

	str_randomize(&(n.str));
	return n;
}

void node_free(Node*n)
{
	if(!n)return;

	for(size_t i=0;i<n->nodes.size;++i)
		node_free(((Node*)n->nodes.buffer)+i);

	if(n->str.buffer)
		str_free(&n->str);

	if(n->nodes.buffer)
		vec_free(&n->nodes);
}

void node_pushnode(Node*n)
{
	Node t=node_new();
	if(!n)return;

	vec_push(&n->nodes,&t);
}

void node_print(Node*n,int lvl)
{
	for(size_t i=0;i<lvl;++i)
		printf("    ");
	printf("%p: (c:%lu/%lu) s:'%s'\n",
			n,
			n->nodes.size,
			n->nodes.capacity,
			n->str.buffer
		  );
	++lvl;

	if(n->nodes.size>0)
	{
		for(size_t i=0;i<n->nodes.size;++i)
			node_print(((Node*)n->nodes.buffer)+i,lvl);
			//printf("	%p\n",&((Node*)n->nodes.buffer)[i]);
	}
}
