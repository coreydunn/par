#include<stdio.h>
#include"state.h"
#include"err.h"

State state;

void cleanquit(int exitval)
{
	puts("bye");
	state_free(&state);
	exit(exitval);
}

void sighandle(int sig)
{
	if(sig==SIGINT)
		cleanquit(0);
}

State state_new(void)
{
	State st=(State){
		.parser=parser_new(),
		.input_buffer=str_new(),
		.lexer=lex_new(),
		.errors=vec_new(sizeof(Err)),
		.infile=stdin,
	};
	return st;
}

void state_free(State*st)
{
	if(!st)
	{
		fprintf(stderr,"error: NULL State pointer passed to state_free\n");
		return;
	}

	lex_free(&state.lexer);
	parser_free(&state.parser);
	str_free(&state.input_buffer);
}
