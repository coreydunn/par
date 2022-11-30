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
		.lexer=lex_new(),
		.parser=parser_new(),
		.input_buffer=str_new(),
		.infile=stdin,
		.outfile=NULL,
	};
	return st;
}

void state_free(State*st)
{
	if(!st)
	{
		err_log("NULL State pointer passed to state_free");
		return;
	}

	lex_free(&st->lexer);
	parser_free(&st->parser);
	str_free(&st->input_buffer);
}
