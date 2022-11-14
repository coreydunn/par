#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"

//#define vec_pushn(v,n) do{Node x=n;vec_push(v,&x);}while(0)

typedef struct Node
{
	Str str;
	Vec nodes;
} Node;

Node node_new(void);
void node_free(Node*n);
void node_pushnode(Node*n);
void node_print(Node*n,int lvl);
