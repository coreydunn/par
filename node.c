#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"node.h"
#include"tok.h"

Node node_new(void)
{
	Node n={
		.nodes=vec_new(sizeof(Node)), // Empty Vec
		.tokens=vec_new(sizeof(Tok)), // Empty Vec
	};

	return n;
}

void node_free(Node*n)
{
	if(!n)return;

	for(size_t i=0;i<n->nodes.size;++i)
		node_free(((Node*)n->nodes.buffer)+i);

	if(n->nodes.buffer)
		vec_free(&n->nodes);

	if(n->tokens.buffer)
		vec_free(&n->tokens);
}

Node*node_pushnode(Node*n)
{
	Node t=node_new();
	if(!n)return NULL;

	vec_push(&n->nodes,&t);
	return &((Node*)n->nodes.buffer)[n->nodes.size-1];
}

void node_print(Node*n,int lvl)
{
	for(size_t i=0;i<lvl;++i)
		printf("    ");
	printf("%p: (c:%lu/%lu) (t: %lu/%lu)",
			n,
			n->nodes.size,
			n->nodes.capacity,
			n->tokens.size,
			n->tokens.capacity
		  );

	// Print tokens
	printf(" [");
	for(size_t i=0;i<n->tokens.size;++i)
	{
		printf("'%s'",((Tok*)n->tokens.buffer)[i].str.buffer);
		if(i<n->tokens.size-1)
			printf(", ");
	}
	printf("]\n");

	++lvl;

	if(n->nodes.size>0)
	{
		for(size_t i=0;i<n->nodes.size;++i)
			node_print(((Node*)n->nodes.buffer)+i,lvl);
	}
}
