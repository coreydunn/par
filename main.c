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
		.string=str_new(),
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
		char buffer[1024]={0};
		size_t count=0;

		count=fread(buffer,1,1000,par.infile);
		buffer[count]=0;
		str_append(&par.string,buffer);

		fclose(par.infile);
	}

	// Lex --> Parse --> Print
	if(par.string.buffer)
	{
		lex_string(&par.lexer,par.string.buffer);
		str_free(&par.string);
		parser_tokens(&par.parser,&par.lexer.tokens);
		lex_free(&par.lexer);
		pnode_print(&par.parser.root,0);
	}

	// Free memory and leave
	cleanup();
}

void cleanup(void)
{
	puts("bye");
	lex_free(&par.lexer);
	pnode_free(&par.parser.root);
	str_free(&par.string);
}

void sighandle(int sig)
{
	if(sig==SIGINT)
	{
		cleanup();
		exit(0);
	}
}
