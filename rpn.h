#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>

#define VSIZE 256

struct vec{int b[VSIZE];int n;};
struct state{struct vec nums;struct vec ops;};

int rpn_pop(struct vec*v);
void rpn_math(struct state*st);
void rpn_parse(struct state*st,char*string);
void rpn_print(struct vec*v,char*fmt);
void rpn_push(struct vec*v,int c);
