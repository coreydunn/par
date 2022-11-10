#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"vec.h"
#include"lex.h"
#include"str.h"

int main(int argc,char**argv)
{
	Str string=str_new();
	Lexer lexer=lex_new();

	if(argc<2)
		str_assign(&string,"x 12 \"fishes\" trees		_Abbc98zy 9999");
	else
		str_assign(&string,argv[1]);

	for(size_t i=0;i<lexer.lexemes.size;++i)
		printf("[%lu] type: %u\n",i,((Lexeme*)vec_at(&lexer.lexemes,i))->type);

	lex_string(&lexer,string.buffer);

	printf("lex_string: ");
	str_print(&string);
	printf("lexemes: ");
	lex_print(&lexer);

	lex_free(&lexer);
	str_free(&string);
}
