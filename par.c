#include"par.h"
#include"lex.h"

void par_tokens(Par*p,Vec*tokens)
{

	for(size_t i=0;i<tokens->size;++i)
	{

		Tok tok=*(Tok*)vec_at(tokens,i);
		printf("%s",tok.str.buffer);
		switch(p->mode)
		{
			case STATEMENT:
			default:
				switch(tok.type)
				{

					case IDENTIFIER:
					case OPERATOR:
						if(strcmp(tok.str.buffer,";")==0)
							printf(" # end statement\n");

					case STRING:
					case KEYWORD:
					default:
						break;

				}
				//printf("%s",((Tok*)tokens->buffer)[i].str.buffer);
				break;
		}
	}

	printf("\n");
}
