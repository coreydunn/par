#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include"vec.h"

#define VECDEFSIZE 64
#define VECVERBOSE true

#define vec_pushi(v,i) do{int x=i;vec_push((v),&x);}while(0)
#define vec_pushc(v,c) do{char x=c;vec_push((v),&x);}while(0)
#define vec_pushf(v,f) do{float x=f;if(!(v)->isfloat)(v)->isfloat=true;vec_push((v),&x);}while(0)
#define vec_pushs(v,s) do{Str x=str_new();str_assign(&x,s);vec_push((v),&x);}while(0)

typedef struct Vec
{
	void*buffer;
	size_t unitsize;
	size_t size;
	size_t capacity;
	bool isfloat;
} Vec;

Vec vec_new(size_t unitsize);
void vec_free(Vec*v);
void vec_grow(Vec*v);
void vec_pop(Vec*v);
void vec_print(Vec*v,const char*fmt);
void vec_push(Vec*v,void*d);
void*vec_at(Vec*v,size_t i);
