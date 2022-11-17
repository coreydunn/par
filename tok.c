#include<stdio.h>
#include"tok.h"
#include"str.h"

Tok tok_new(void)
{
	Tok t={
		.str=str_new(),
		.type=0,
		.line=1,
	};

	return t;
}

void tok_free(Tok*t)
{
	if(!t)return;
	str_free(&t->str);
}
