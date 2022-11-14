#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"vec.h"
#include"lex.h"
#include"str.h"
#include"pnode.h"

int main(int argc,char**argv)
{
	PNode root=pnode_new();
	Str string=str_new();
	Lexer lexer=lex_new();

	// Determine string to lex/parse
	if(argc<2)
		str_assign(&string,"name=\"sam\"; x=3; number=77*x;");
		//str_assign(&string,"while(true)\n{\nx=253;\ny=\"string\";\n}");
	else
		str_assign(&string,argv[1]);

	for(size_t i=0;i<lexer.tokens.size;++i)
		printf("[%lu] type: %u\n",i,((Tok*)vec_at(&lexer.tokens,i))->type);

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
	pnode_pushnode(&root);
	pnode_pushnode(&root);
	pnode_pushnode(&root);

	// Copy tokens
	for(size_t i=0;i<lexer.tokens.size;++i)
		vec_pushta(&root.tokens,(((Tok*)lexer.tokens.buffer)[i].str.buffer));

	pnode_print(&root,0);
	pnode_free(&root);
	pnode_print(&root,0);

	lex_free(&lexer);
	str_free(&string);
}
