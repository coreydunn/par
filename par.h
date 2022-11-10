#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"lex.h"
#include"str.h"

enum PARTYPE {/*NONE,*/ STATEMENT=1, EXPRESSION};

typedef struct Par
{
	uint32_t mode;
} Par;

void par_tokens(Par*p,Lexer*l);
