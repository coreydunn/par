#include<stdio.h>
#include"err.h"

Err err_new(void)
{
	Err e={
		.msg=str_new(),
		.id=0,
	};
	return e;
}

void err_free(Err*e)
{
	if(!e)
	{
		fprintf(stderr,"error: invalid Err struct passed to err_free\n");
		return;
	}
	if(e->str.buffer)
		str_free(&e->str);
}
