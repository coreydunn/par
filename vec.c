#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include"vec.h"

Vec vec_new(size_t unitsize)
{
	Vec v={
		.buffer=NULL,
		.unitsize=unitsize,
		.size=0,
		.capacity=0,
	};

	return v;
}

void vec_print(Vec*v,const char*fmt)
{
	printf("%p: sz=%lu (%lu/%lu): [",v,v->unitsize,v->size,v->capacity);
	for(size_t i=0;i<v->size;++i)
	{

		if(v->isfloat)
		{
			if(v->unitsize==4)
				printf(fmt,((float*)v->buffer)[i]);
			else if(v->unitsize==8)
				printf(fmt,((double*)v->buffer)[i]);
		}
		else
			printf(fmt,((uint8_t*)v->buffer)[i]);

		if(i<v->size-1)
			printf(", ");
	}
	printf("]\n");
}

void vec_free(Vec*v)
{
	if(v->buffer)
		free(v->buffer);
	v->buffer=NULL;
	v->size=0;
	v->capacity=0;
	v->unitsize=0;
}

void vec_grow(Vec*v)
{
	if(!v)return;
	if(!v->unitsize)return;
	if(!v->buffer)
		v->buffer=malloc(VECDEFSIZE*v->unitsize);
	else
		v->buffer=realloc(v->buffer,(v->capacity+VECDEFSIZE)*v->unitsize);

	if(!v->buffer)
	{
		v->buffer=NULL;
		v->size=0;
		v->capacity=0;
		v->unitsize=0;
	}

	v->capacity+=VECDEFSIZE;
}

void vec_push(Vec*v,void*d)
{
	if(!v)return;
	if(!v->unitsize)return;

	if(!v->buffer||v->size+1>v->capacity)
		vec_grow(v);

	if(!v->buffer)return;
	memcpy(v->buffer+(v->size)*v->unitsize,d,v->unitsize);
	++v->size;
}

void vec_pop(Vec*v)
{
	if(!v)return;
	if(!v->size)return;
	--v->size;
}

void*vec_at(Vec*v,size_t i)
{
	return (v->buffer)+(i*v->unitsize);
}
