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
	{
		s->buffer=malloc(n);
		if(s->buffer)
			s->buffer[0]=0;
	}
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
		str_grow(s,n+STRDEFSIZE);
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
		str_grow(s,s->size+n+STRDEFSIZE);
	if(!s->buffer)
		return;
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

int search(int*a,int n,int v)
{
	int b=0,e=n-1,m=(e-b)/2;

	do
	{

		if(a[m]==v)
			return m;

		if(a[m]<v)
		{
			if(e==b){m=-1;break;}
			else b=m+1,m=(e-b)/2+b;
		}
		else if(a[m]>v)
		{
			if(e==b){m=-1;break;}
			else e=m-1,m=(e-b)/2+b;
		}

	}while(true);

	return m;
}

// Binary search of array of char pointers
int cstr_search(char**a,int n,char*v)
{
	int b=0,e=n-1,m=(e-b)/2;

	do
	{

		if(strcmp(a[m],v)==0)
			return m;

		if(strcmp(a[m],v)<0)
		{
			if(e<=b){m=-1;break;}
			else b=m+1,m=(e-b)/2+b;
		}

		else if(strcmp(a[m],v)>0)
		{
			if(e<=b){m=-1;break;}
			else e=m-1,m=(e-b)/2+b;
		}

	}while(true);

	return -1;
}

// Sort array of char pointers
void cstr_sort(char**a,size_t n)
{
	char*t;
	for(size_t i=0;i<n-1;++i)
		for(size_t j=i+1;j<n;++j)
			if(strcmp(a[i],a[j])>0)
				t=a[i],
				a[i]=a[j],
				a[j]=t;
}
