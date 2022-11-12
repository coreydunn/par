#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"vec.h"
#include"str.h"
#include"tok.h"

#define vec_pushpn(v,pn) do{ParseNode x=pn;vec_push(v,&x);}while(0)

enum PARTYPE {STATEMENT, EXPRESSION, ASSIGNMENT, IFSTATEMENT};

// Smallest unit of parsing tree grammar
typedef struct ParseNode
{
	Vec tokens;
	Vec children;
	uint32_t type;
} ParseNode;

typedef struct Parser
{
	ParseNode root;
	uint32_t mode;
} Parser;

ParseNode par_new(void);
void par_free(ParseNode*pn);
void par_push(ParseNode*pn);
void par_print(ParseNode*p);
void par_tokens(Parser*p,Vec*tokens);
