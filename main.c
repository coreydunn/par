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

	// Determine string to lex/parse
	if(argc<2)
		str_assign(&string,"name=\"sam\"; x=3; number=77*x;");
		//str_assign(&string,"while(true)\n{\nx=253;\ny=\"string\";\n}");
	else
		str_assign(&string,argv[1]);

	// LEX -----
	// Identify lexemes in string
	lex_string(&lexer,string.buffer);

	// Print results
	printf("lex_string: ");
	str_print(&string);
	printf("tokens: ");
	lex_print(&lexer);

	// PARSE -----
	// Create Parsing tree
	parser_tokens(&parser,&lexer.tokens);

	lex_free(&lexer);

	pnode_print(&parser.root,0);

	// Free memory and leave
	pnode_free(&parser.root);
	//lex_free(&lexer);
	str_free(&string);
}
