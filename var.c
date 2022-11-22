#include"var.h"

Var var_new(void)
{
	Var v={
		.name=str_new(),
		.type=0,
	};
	return v;
}

void var_free(Var*v)
{
	if(!v)return;
	str_free(&v->name);
}
