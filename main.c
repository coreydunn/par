#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"state.h"

int main(int argc,char**argv)
{
	state=state_new();

	// Ensure cleanup is called
	signal(SIGINT,sighandle);

	// Determine string to lex/parse
	if(argc>1)
	{
		state.infile=fopen(argv[1],"r");
		if(!state.infile)
			err_log("failed to open infile '%s'",argv[1]);
	}

	// Read input file into buffer
	if(state.infile)
	{
		char file_buffer[1024]={0};
		size_t count=0;

		count=fread(file_buffer,1,1000,state.infile);
		file_buffer[count]=0;
		str_append(&state.input_buffer,file_buffer);

		fclose(state.infile);
	}

	// Lex --> Parse --> Print
	if(state.input_buffer.buffer)
	{
		lex_string(&state.lexer,state.input_buffer.buffer);
		str_free(&state.input_buffer);
		//lex_print(&state.lexer);
		parser_parse(&state.parser,&state.lexer.tokens);
		lex_free(&state.lexer);
		pnode_print(&state.parser.root,0);
		//pnode_print_brief(&state.parser.root,0);
	}

	{
	  Str fn=str_new();
	  if(state.infile!=stdin)
	    fn=str_basename(argv[1]);
	  else
	    str_assign(&fn,"a.asm");
	  str_append(&fn,".asm");
	  
		state.outfile=fopen(fn.buffer,"w");
		if(!state.outfile)state.outfile=stdout;
		if(!state.outfile){err_log("cannot open outfile '%s'","a.out");state.outfile=stdout;}

		gen_x86_64(&state.parser.root,state.outfile);

		if(state.outfile)
			fclose(state.outfile);
		str_free(&fn);
	}

	// Free memory and leave
	state_free(&state);
	cleanquit(0);
}
