#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"lex.h"
#include"pnode.h"
#include"reg.h"
#include"str.h"
#include"vec.h"

int main(int argc,char**argv)
{
	Parser parser=parser_new();
	Str string=str_new();
	Lexer lexer=lex_new();
	FILE*infile=stdin;

	// Determine string to lex/parse
	if(argc>1)
	{
		infile=fopen(argv[1],"r");
		if(!infile)
			fprintf(stderr,"error: failed to open infile '%s'\n",argv[1]);
	}

	// Read input file into buffer
	if(infile)
	{
		char buffer[1024]={0};
		size_t count=0;

		count=fread(buffer,1,1000,infile);
		buffer[count]=0;
		str_append(&string,buffer);

		fclose(infile);
	}


	// Lex --> Parse --> Print
	if(string.buffer)
	{
		lex_string(&lexer,string.buffer);
		str_free(&string);
		parser_tokens(&parser,&lexer.tokens);
		lex_free(&lexer);
		pnode_print(&parser.root,0);
	}

	// Free memory and leave
	lex_free(&lexer);
	pnode_free(&parser.root);
	//lex_free(&lexer);
	str_free(&string);
}
