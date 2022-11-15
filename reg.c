#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"vec.h"
#include"reg.h"

void reg_match(Reg*r,char*p,char*s)
{
	//char*p="\\d";
	//char*s="1__23";
	//Vec r->matches=vec_new(sizeof(Match));

	// Iterate through pattern
	for(size_t j=0;p[j];++j)
	{
		//printf("((%lu))\n",j);
		if(strcmp(p+j,"\\d")==0)
		{
			++j;
			for(size_t i=0;s[i];++i)
				if(isdigit(s[i]))
				{
					Match m={.length=1,.str=s+i};
					vec_push(&r->matches,&m);
				}
		}
	}
}

void reg_print(Reg*r)
{
	if(!r)return;
	printf("%p [",&r->matches);
	for(size_t i=0;i<r->matches.size;++i)
	{
		Match*m=((Match*)r->matches.buffer)+i;
		//char*string=m->str;

		printf("(%lu/",m->length);
		for(size_t j=0;j<m->length;++j)
			printf("%c",m->str[j]);
		printf(")");
		if(i<r->matches.size-1)
			printf(", ");
	}
	printf("]\n");
}

Reg reg_new(void)
{
	Reg r={.mode=MAGIC,.matches=vec_new(sizeof(Match))};
	return r;
}

void reg_free(Reg*r)
{
	if(!r)return;
	vec_free(&r->matches);
}
