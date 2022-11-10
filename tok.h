#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lex.h"

typedef struct Tok
{
	Str str;
	uint32_t type;
} Tok;
