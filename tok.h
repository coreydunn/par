#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lex.h"

#define vec_pusht(v,s) do{Tok x=(Tok){.str=str_new(),type=0};vec_push((v),&x);}while(0)

typedef struct Tok
{
	Str str;
	uint32_t type;
} Tok;
