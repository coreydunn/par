#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include"lex.h"
#include"tok.h"
#include"str.h"

#define vec_pushst(v,s) do{Statement x=s;vec_push(v,&x);}while(0)

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
enum PARTYPE {STATEMENT, EXPRESSION, ASSIGNMENT, IFSTATEMENT};

// This struct describes the smallest
// grammatical unit for our parser.
// Reference to a Vec of Tok structs
// starting at first and ending at last.
// Type is from PARTYPE.
typedef struct Statement
{
	size_t type;
	size_t first;
	size_t last;
} Statement;

// statements should be a Vec of Statements structs
// stack should be a Vec of uint32_t for the mode
typedef struct Par
{
	Vec statements;
	Vec stack;
	uint32_t mode;
} Par;

// tokens should be a vector of Tok structs
void par_tokens(Par*p,Vec*tokens);
