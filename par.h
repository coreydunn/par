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
#include"var.h"

// State for par program
typedef struct State
{
	Lexer lexer;
	Parser parser;
	Str input_buffer;
	FILE*infile;
} State;

void cleanquit(int exitval);
void sighandle(int sig);
