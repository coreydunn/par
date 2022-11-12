#include<stdio.h>
#include<stdint.h>
#include"vec.h"
#include"str.h"
#include"ptree.h"

void par_print(ParseNode*p)
{
	printf("%p: [",p);
	for(size_t i=0;i<p->children.size;++i)
	{
		//printf("%lu",((ParseNode*)p->children)[i].type);
		printf("%lu:%u",i,((ParseNode*)p->children.buffer)[i].type);
		if(i<p->children.size-1)
			printf(", ");
	}
	printf("]\n");
}

int main(void)
{
	ParseNode pn=par_new();

	printf("empty: ");
	par_print(&pn);
	vec_pushpn(&pn.children,par_new());
	vec_pushpn(&pn.children,par_new());
	vec_pushpn(&pn.children,par_new());
	vec_pop(&pn.children);

	printf("filled: ");
	par_print(&pn);

	par_free(&pn);
}
