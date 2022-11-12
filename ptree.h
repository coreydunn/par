#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"vec.h"
#include"str.h"
#include"tok.h"

typedef struct ParseNode
{
	Vec tokens;
	Vec children;
} ParseNode;

typedef struct Parser
{
	ParseNode tree;
} Parser;

ParseNode par_new(void);
void par_free(ParseNode*pn);
