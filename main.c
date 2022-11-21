#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"par.h"

static State par;

int main(int argc,char**argv)
{
	par=(State){
		.parser=parser_new(),
		.input_buffer=str_new(),
		.lexer=lex_new(),
		.infile=stdin,
	};

	// Ensure cleanup is called
	signal(SIGINT,sighandle);

	// Determine string to lex/parse
	if(argc>1)
	{
		par.infile=fopen(argv[1],"r");
		if(!par.infile)
			fprintf(stderr,"error: failed to open infile '%s'\n",argv[1]);
	}

	// Read input file into buffer
	if(par.infile)
	{
		char file_buffer[1024]={0};
		size_t count=0;

		count=fread(file_buffer,1,1000,par.infile);
		file_buffer[count]=0;
		str_append(&par.input_buffer,file_buffer);

		fclose(par.infile);
	}

	// Lex --> Parse --> Print
	if(par.input_buffer.buffer)
	{
		lex_string(&par.lexer,par.input_buffer.buffer);
		str_free(&par.input_buffer);
		lex_print(&par.lexer);
		parser_parse(&par.parser,&par.lexer.tokens);
		lex_free(&par.lexer);
		pnode_print(&par.parser.root,0);
	}

	// Free memory and leave
	cleanquit(0);
}

void cleanquit(int exitval)
{
	puts("bye");
	lex_free(&par.lexer);
	pnode_free(&par.parser.root);
	str_free(&par.input_buffer);
	exit(exitval);
}

void sighandle(int sig)
{
	if(sig==SIGINT)
		cleanquit(0);
}
