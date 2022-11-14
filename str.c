#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include"str.h"

Str str_new(void)
{
	Str s={
		.buffer=NULL,
		.capacity=0,
		.size=0,
	};

	return s;
}

void str_free(Str*s)
{
	if(!s)return;
	if(s->capacity)
		free(s->buffer);
	s->capacity=0;
	s->size=0;
	s->buffer=NULL;
}

void str_grow(Str*s,size_t n)
{
	if(!s)return;

	if(!s->buffer)
		s->buffer=malloc(n);
	else
		s->buffer=realloc(s->buffer,s->capacity+n);

	if(!s->buffer)return;
	s->capacity=n;
}

void str_assign(Str*s,char*c)
{
	size_t n;

	if(!s)return;
	n=strlen(c)+1;
	if(s->capacity<n)
		str_grow(s,n);
	strcpy(s->buffer,c);
	s->size=n-1;
}

void str_print(Str*s)
{
	printf("%p: (%lu/%lu) '%s'\n",s,s->size,s->capacity,s->buffer);
}

void str_append(Str*s,char*c)
{
	size_t n;

	if(!s||!c)return;

	n=strlen(c)+1;
	if(s->capacity<s->size+n)
		str_grow(s,s->size+n);
	strcat(s->buffer,c);
	s->size+=n-1;

}

void str_tr(Str*s,char a,char b)
{
	if(!s)return;
	for(size_t i=0;i<s->size;++i)
		if(s->buffer[i]==a)
			s->buffer[i]=b;
}

void str_clear(Str*s)
{
	if(!s)return;
	s->size=0;
	if(s->buffer)
		s->buffer[0]='\0';
}

Str str_newa(char*c)
{
	Str n=str_new();

	if(c)
		str_assign(&n,c);

	return n;
}

void str_randomize(Str*s)
{
	size_t length=rand()%8+1;
	bool vowell=false;
	char*v="aeiou";
	char*c="bcdfghjklmnpqrstvwxyz";
	size_t i=0;

	str_assign(s,"........\0");
	for(;i<length;++i)
	{
		s->buffer[i]=(vowell)?(v[rand()%5]):(c[rand()%21]);
		vowell=!vowell;
	}
	s->buffer[i]='\0';
}
