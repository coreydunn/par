#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"vec.h"
#include"lex.h"
#include"str.h"
#include"node.h"

int main(int argc,char**argv)
{
	Str string=str_new();
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
	printf("~~~~~~ Test Node\n");
	{
		// Create Parsing tree
		Node root=node_new();
		node_pushnode(&root);
		node_pushnode(&root);
		node_pushnode(&root);

		// Copy tokens
		for(size_t i=0;i<lexer.tokens.size;++i)
			vec_pushta(&root.tokens,(((Tok*)lexer.tokens.buffer)[i].str.buffer));

		node_print(&root,0);
		node_free(&root);
		node_print(&root,0);
	}
	printf("~~~~~~\n");

	lex_free(&lexer);
	str_free(&string);
}
