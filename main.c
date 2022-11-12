#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"vec.h"
#include"lex.h"
#include"str.h"
#include"par.h"
#include"ptree.h"

int main(int argc,char**argv)
{
	Str string=str_new();
	Par par;
	Lexer lexer=lex_new();

	// Determine string to lex/parse
	if(argc<2)
		str_assign(&string,"while(true)\n{\nx=253;\ny=\"string\";\n}");
	else
		str_assign(&string,argv[1]);

	for(size_t i=0;i<lexer.tokens.size;++i)
		printf("[%lu] type: %u\n",i,((Tok*)vec_at(&lexer.tokens,i))->type);

	// LEX
	// Identify lexemes in string
	lex_string(&lexer,string.buffer);

	// Print results
	printf("lex_string: ");
	str_print(&string);
	printf("tokens: ");
	lex_print(&lexer);

	// PARSE
	// Parse lexemes
	par_tokens(&par,&lexer.tokens);

	// ParseNode
	printf("!!!test ParseNode\n");
	{
		ParseNode pn=par_new();
		printf("ParseNode: ");
		printf("tokens: (%lu/%lu) ",pn.tokens.size,pn.tokens.capacity);
		printf("children: (%lu/%lu)",pn.children.size,pn.children.capacity);
		printf("\n");
		par_free(&pn);
	}
	printf("!!!\n\n");

	lex_free(&lexer);
	str_free(&string);
}
