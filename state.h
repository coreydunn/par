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
#include"mem.h"
#include"err.h"
#include"x86_64.h"

// State for par program
typedef struct State
{
	Lexer lexer;
	Parser parser;
	Str input_buffer;
	Vec errors;
	FILE*infile;
	FILE*outfile;
} State;

extern State state;

State state_new(void);
void state_free(State*st);
void state_print_errors(State*st);

void cleanquit(int exitval);
void sighandle(int sig);
