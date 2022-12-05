#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"tok.h"
#include"mem.h"

//#define vec_pushn(v,n) do{PNode x=n;vec_push(v,&x);}while(0)

typedef enum PARTYPE {PNONE, PEMPTY, PEXPRESSION, PSTATEMENT, PASSIGNMENT, PIF, PCOMMENT, PBLOCK, PWHILE, PVARDECL, PFUNDECL, PRET, PCALL, } PARTYPE;

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
	size_t firstline;
} PNode;

typedef struct Parser
{
	PNode root;
	uint32_t mode;
	Vec mem_locations;
} Parser;

PNode pnode_new(void);
PNode*pnode_pushnode(PNode*n);
Parser parser_new(void);
void parser_free(Parser*p);
void parser_parse(Parser*p,Vec*t);
void parser_tokens(Parser*p,Vec*t);
void pnode_free(PNode*n);
void pnode_print(PNode*n,size_t lvl);
void pnode_print_brief(PNode*n,size_t lvl);
