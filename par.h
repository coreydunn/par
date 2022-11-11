#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include"lex.h"
#include"tok.h"
#include"str.h"

/******
 * Grammar:
 * STATEMENT:	EXPRESSION ';'
 * 				|IDENTIFIER(ARGLIST){STATEMENT}
 * 				|'if'(EXPRESSION){STATEMENT}
 * 				|'while'(EXPRESSION){STATEMENT}
 * EXPRESSION:	INTEGER
 * 				|IDENTIFIER
 * 				|INTEGER OPERATOR EXPRESSION
 * 				|IDENTIFIER OPERATOR EXPRESSION
 * ARGLIST:		TYPENAME IDENTIFIER
 * 				|TYPENAME IDENTIFIER ',' ARGLIST
 ******/
enum PARTYPE {STATEMENT, EXPRESSION};

typedef struct Par
{
	uint32_t mode;
} Par;

// tokens should be a vector of Tok structs
void par_tokens(Par*p,Vec*tokens);
