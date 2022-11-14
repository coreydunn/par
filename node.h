#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"

//#define vec_pushn(v,n) do{Node x=n;vec_push(v,&x);}while(0)

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
typedef struct Node
{
	Vec tokens;
	Vec nodes;
} Node;

Node node_new(void);
Node*node_pushnode(Node*n);
void node_free(Node*n);
void node_print(Node*n,int lvl);
