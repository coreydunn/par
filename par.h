#pragma once

#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"lex.h"
#include"pnode.h"
#include"reg.h"
#include"str.h"
#include"vec.h"

// State for par program
typedef struct State
{
	Lexer lexer;
	Parser parser;
	Str string;
	FILE*infile;
} State;

void cleanup(void);
void sighandle(int sig);
