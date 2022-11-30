#pragma once

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include"str.h"

enum MEMTYPE {VI32, VU32, VSTR, };

typedef struct Mem
{
	Str name;
	size_t type;
} Mem;

Mem mem_new(void);
void mem_free(Mem*v);
