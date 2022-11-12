#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"vec.h"
#include"lex.h"
#include"str.h"
//#include"par.h"
#include"ptree.h"

int main(int argc,char**argv)
{
	Str string=str_new();
	//Par par;
	Lexer lexer=lex_new();

	// Determine string to lex/parse
	if(argc<2)
		str_assign(&string,"name=\"sandwiches or what\";");
		//str_assign(&string,"while(true)\n{\nx=253;\ny=\"string\";\n}");
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
	//par_tokens(&par,&lexer.tokens);

	// ParseNode
	printf("!!! test ParseNode\n");
	{
		Parser p={.mode=0,.root=par_new()};

		// Push tokens to parser tree root node
		par_push(&p.root);
		//vec_push(&p.root.tokens,"what now");
		par_push(&p.root);
		par_push(&p.root);

		// Parse tokens
		par_tokens(&p,&lexer.tokens);

		// Print, free, print
		printf("push: ");par_print(&p.root);
		par_free(&p.root);
		printf("free: ");par_print(&p.root);
	}
	printf("!!!\n");

	lex_free(&lexer);
	str_free(&string);
}
