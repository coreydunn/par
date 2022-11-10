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
	//Vec keywords=vec_new(sizeof(Str));

	if(argc<2)
		str_assign(&string,"x=253; y=\"string\";");
	else
		str_assign(&string,argv[1]);

	for(size_t i=0;i<lexer.tokens.size;++i)
		printf("[%lu] type: %u\n",i,((Lexeme*)vec_at(&lexer.tokens,i))->type);

	lex_string(&lexer,string.buffer);

	//printf("keywords: %p: [",&keywords);
	//for(size_t i=0;i<keywords.size;++i)
	//{
		//printf("'%s'",((Str*)keywords.buffer)[i].buffer);
		//if(i<keywords.size-1)
			//printf(", ");
	//}
	//printf("]\n");

	printf("lex_string: ");
	str_print(&string);
	printf("tokens: ");
	lex_print(&lexer);

	par_tokens(&par,&lexer);

	//for(size_t i=0;i<keywords.size;++i)
		//str_free(((Str*)vec_at(&keywords,i)));
	lex_free(&lexer);
	str_free(&string);
	//vec_free(&keywords);
}
