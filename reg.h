#pragma once

#include<stdio.h>
#include<string.h>
#include"vec.h"

enum REGTYPE {MAGIC, NOMAGIC};

typedef struct Match
{
	size_t length;
	char*str;
} Match;

typedef struct Reg
{
	uint32_t mode;
	Vec matches; // Match Vec
} Reg;

Reg reg_new(void);
void reg_free(Reg*r);
void reg_match(Reg*r,char*p,char*s);
void reg_print(Reg*r);
