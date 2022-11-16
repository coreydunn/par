#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"pnode.h"
#include"tok.h"

Parser parser_new(void)
{
	Parser p={
		.root=pnode_new(),
		.mode=0,
	};
	return p;
}

PNode pnode_new(void)
{
	PNode n={
		.pnodes=vec_new(sizeof(PNode)), // Empty Vec
		.tokens=vec_new(sizeof(Tok)), // Empty Vec
		.parentnode=NULL,
	};

	return n;
}

void pnode_free(PNode*n)
{
	if(!n)return;

	for(size_t i=0;i<n->pnodes.size;++i)
		pnode_free(((PNode*)n->pnodes.buffer)+i);

	if(n->pnodes.buffer)
		vec_free(&n->pnodes);

	if(n->tokens.buffer)
		vec_free(&n->tokens);
}

PNode*pnode_pushnode(PNode*n)
{
	PNode t=pnode_new();
	if(!n)return NULL;

	vec_push(&n->pnodes,&t);
	return &((PNode*)n->pnodes.buffer)[n->pnodes.size-1];
}

void pnode_print(PNode*n,size_t lvl)
{
	for(size_t i=0;i<lvl;++i)
		printf("    ");
	printf("%p: (c:%lu/%lu) (t: %lu/%lu)",
			n,
			n->pnodes.size,
			n->pnodes.capacity,
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

	if(n->pnodes.size>0)
	{
		for(size_t i=0;i<n->pnodes.size;++i)
			pnode_print(((PNode*)n->pnodes.buffer)+i,lvl);
	}
}

// Parse Vec of Tok
void parser_tokens(Parser*p,Vec*t)
{
	if(!p)return;
	if(!t)return;

	// Create child nodes
	//pnode_pushnode(&p->root);
	//pnode_pushnode(&p->root);
	//pnode_pushnode(&p->root);

	// Copy tokens
	for(size_t i=0;i<t->size;++i)
	{
		Tok*tok=&((Tok*)t->buffer)[i];
		if(tok->type!=LCOMMENT)
			vec_pushta(&p->root.tokens,tok->str.buffer);
		//vec_pushta(&parser.root.tokens,(((Tok*)lexer.tokens.buffer)[i].str.buffer));
	}

}
