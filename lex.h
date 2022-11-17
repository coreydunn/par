#pragma once

#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"vec.h"
#include"str.h"
#include"tok.h"

#define vec_pushl(v,l) do{Tok x=l;vec_push(v,&x);}while(0)

enum LEXTYPE {NONE, IDENTIFIER, INTEGER, FLOAT, STRING, OPERATOR, KEYWORD, LCOMMENT};
extern char*lextype_names[];

//typedef struct Tok
//{
	//Str str;
	//uint32_t type;
//} Tok;

typedef struct Lexer
{
	Vec tokens;
	uint32_t mode;
} Lexer;

Lexer lex_new(void);
size_t lex_strchrcount(char*str,char c);
void lex_free(Lexer*l);
void lex_print(Lexer*l);
void lex_string(Lexer*l,char*s);
