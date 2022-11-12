#include<stdio.h>
#include<stdlib.h>
#include"vec.h"
#include"str.h"
#include"ptree.h"

ParseNode par_new(void)
{
	ParseNode pn={
		.tokens=vec_new(sizeof(Tok)),
		.children=vec_new(sizeof(ParseNode)),
		.type=7,
	};

	return pn;
}

void par_free(ParseNode*pn)
{
	if(!pn)return;

	if(pn->children.buffer)
	{
		for(size_t i=0;i<pn->tokens.size;++i)
			str_free(&((Tok*)pn->tokens.buffer)[i].str);
		vec_free(&pn->tokens);
		vec_free(&pn->children);
	}
}

void par_push(ParseNode*pn)
{
	if(!pn)return;
	vec_pushpn(&pn->children,par_new());
}

void par_print(ParseNode*p)
{
	printf("%p: [",p);
	for(size_t i=0;i<p->children.size;++i)
	{
		//printf("%lu:%u",i,((ParseNode*)p->children.buffer)[i].type);
		for(size_t j=0;j<((ParseNode*)p->children.buffer)[i].tokens.size;++j)
			printf("%lu:%s",i,((Tok*)(((ParseNode*)p->children.buffer)[i].tokens).buffer)[j].str.buffer);
		if(i<p->children.size-1)
			printf(", ");
	}
	printf("]\n");
}

