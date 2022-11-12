#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"vec.h"
#include"lex.h"
#include"str.h"
#include"par.h"

int main(int argc,char**argv)
{
	Str string=str_new();
	Par par;
	Lexer lexer=lex_new();

	if(argc<2)
		str_assign(&string,"while(true)\n{\nx=253;\ny=\"string\";\n}");
	else
		str_assign(&string,argv[1]);

	for(size_t i=0;i<lexer.tokens.size;++i)
		printf("[%lu] type: %u\n",i,((Tok*)vec_at(&lexer.tokens,i))->type);

	lex_string(&lexer,string.buffer);

	printf("lex_string: ");
	str_print(&string);
	printf("tokens: ");
	lex_print(&lexer);

	par_tokens(&par,&lexer.tokens);

	lex_free(&lexer);
	str_free(&string);
}
