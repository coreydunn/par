#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"tok.h"

//#define vec_pushn(v,n) do{PNode x=n;vec_push(v,&x);}while(0)

enum PARTYPE {EXPRESSION, STATEMENT, ASSIGNMENT, IFSTATEMENT, DECLARATION, COMMENT, CODEBLOCK};

extern char*partype_names[];

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
	struct PNode*parentnode;
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
Parser parser_new(void);
void parser_tokens(Parser*p,Vec*t);
void pnode_free(PNode*n);
void pnode_print(PNode*n,size_t lvl);
void pnode_print_brief(PNode*n,size_t lvl);
