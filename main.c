#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"state.h"

int main(int argc,char**argv)
{
	bool showparsetree=false;
	bool showparsetreebrief=false;
	bool showtokens=false;

	//state=state_new();
	Vec args=vec_new(sizeof(char*));

	// Ensure cleanup is called
	signal(SIGINT,sighandle);

	// Determine string to lex/parse
	for(int i=1;i<argc;++i)
	{
		if(!argv[i][0])continue;
		if(argv[i][0]=='-')
		{
			if(argv[i][1]=='-')
			{

				// LONG OPTIONS
				if(strcmp("--help",argv[i])==0)
				{
					puts("help yourself");
					cleanquit(0);
				}

				else
				{
					err_log("unrecognized long option '%s'",argv[i]);
					cleanquit(1);
				}
			}

			// SHORT OPTIONS
			else
			{
				for(size_t j=1;argv[i][j];++j)
				{
					switch(argv[i][j])
					{

						case 'p':
							showparsetree=true;
							break;

						case 'b':
							showparsetree=true;
							showparsetreebrief=true;
							break;

						case 't':
							showtokens=true;
							break;

						default:
							err_log("unrecognized short option '%c'",argv[i][j]);
							cleanquit(1);
							break;

					}
				}
			}

			continue;
		}

		// All arguments otherwise treated as filenames
		vec_push(&args,argv+i);
	}

	// Each file in vector
	for(size_t i=0;i<args.size;++i)
	{
		state=state_new();

		state.infile=fopen(((const char**)args.buffer)[i],"r");
		if(!state.infile)
			err_log("failed to open infile '%s'",argv[1]);

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
			if(showtokens)lex_print(&state.lexer);
			parser_parse(&state.parser,&state.lexer.tokens);
			lex_free(&state.lexer);
			if(showparsetree)
			{
				if(showparsetreebrief)
					pnode_print_brief(&state.parser.root,0);
				else
					pnode_print(&state.parser.root,0);
			}
		}

		{
			Str fn=str_new();
			if(state.infile!=stdin)
				fn=str_basename(((char**)args.buffer)[i]);
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

		state_free(&state);
	}

	// Free memory and leave
	state_free(&state);
	vec_free(&args);
	cleanquit(0);
}
