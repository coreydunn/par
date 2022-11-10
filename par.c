#include"par.h"

void par_tokens(Par*p,Lexer*l)
{
	for(size_t i=0;i<l->tokens.size;++i)
	{
		switch(p->mode)
		{
			case STATEMENT:
			default:
			printf("%s ",((Lexeme*)l->tokens.buffer)[i].str.buffer);
		}
	}
	printf("\n");
}
