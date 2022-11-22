#pragma once

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include"str.h"

enum VARTYPE {VI32, VU32, VSTR, };

typedef struct Var
{
	Str name;
	size_t type;
} Var;

Var var_new(void);
void var_free(Var*v);
