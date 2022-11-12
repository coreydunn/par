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
