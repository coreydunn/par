#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"

//#define vec_pushn(v,n) do{PNode x=n;vec_push(v,&x);}while(0)

enum PARTYPE {STATEMENT, EXPRESSION, ASSIGNMENT, IFSTATEMENT};

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
typedef struct PNode
{
	Vec tokens;
	Vec pnodes;
	uint32_t type;
} PNode;

typedef struct Parser
{
	PNode root;
	uint32_t mode;
} Parser;

PNode pnode_new(void);
PNode*pnode_pushnode(PNode*n);
void pnode_free(PNode*n);
void pnode_print(PNode*n,int lvl);
