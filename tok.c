#include<stdio.h>
#include"tok.h"
#include"str.h"

Tok tok_new(void)
{
	Tok t={
		.str=str_new(),
		.type=0,
		.subtype=0,
		.line=1,
	};

	return t;
}

void tok_free(Tok*t)
{
	if(!t)return;
	str_free(&t->str);
}

// Copy everything but string !!!
void tok_copy_nostr(Tok*dst,Tok*src)
{
	if(!dst||!src)return;
	dst->type=src->type;
	dst->subtype=src->subtype;
	dst->line=src->line;
}
