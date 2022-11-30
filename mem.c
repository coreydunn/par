#include"mem.h"

Mem mem_new(void)
{
	Mem v={
		.name=str_new(),
		.type=0,
	};
	return v;
}

void mem_free(Mem*v)
{
	if(!v)return;
	str_free(&v->name);
}
