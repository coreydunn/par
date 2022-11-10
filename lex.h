#pragma once

#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"vec.h"
#include"str.h"

#define vec_pushl(v,l) do{Lexeme x=l;vec_push(v,&x);}while(0)

enum LEXTYPE {NONE, IDENTIFIER, INTEGER, STRING, OPERATOR, KEYWORD};

typedef struct Lexeme
{
	Str str;
	uint32_t type;
} Lexeme;

typedef struct Lexer
{
	Vec lexemes;
	uint32_t mode;
} Lexer;

Lexer lex_new(void);
void lex_free(Lexer*l);
void lex_string(Lexer*l,char*s);
void lex_print(Lexer*l);
