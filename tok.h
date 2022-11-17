#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lex.h"

#define vec_pusht(v) do{Tok x=tok_new();vec_push((v),&x);}while(0)
// create token from C string
#define vec_pushta(v,s) do{Tok x=tok_new();str_assign(&x.str,s);vec_push((v),&x);}while(0)

typedef struct Tok
{
	Str str;
	uint32_t type;
	size_t line;
} Tok;

void tok_free(Tok*t);
Tok tok_new(void);
