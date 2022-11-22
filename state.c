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

	for(size_t i=0;i<st->errors.size;++i)
		err_free(&((Err*)st->errors.buffer)[i]);
	vec_free(&st->errors);
	lex_free(&st->lexer);
	parser_free(&st->parser);
	str_free(&st->input_buffer);
}

void state_print_errors(State*st)
{
	err_print_vec(&st->errors);
}
