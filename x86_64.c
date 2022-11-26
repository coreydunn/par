#include<stdio.h>
#include"x86_64.h"
#include"state.h"

void gen_x86_64(PNode*pn)
{
	Tok*tokens=(Tok*)pn->tokens.buffer;

	switch(pn->type)
	{

		case PFUNDECL:
			printf("global %s\n%s:\n",tokens[0].str.buffer,tokens[0].str.buffer);
			break;

		case PVARDECL:
			printf("xor eax,eax ;%s\n",tokens[0].str.buffer);
			break;

		case PCOMMENT:
				printf(";%s\n",tokens[0].str.buffer);
			break;

		default:
			if(pn->tokens.size>0)
			{
				printf("%s:",partype_names[pn->type]);
				for(size_t i=0;i<pn->tokens.size;++i)
				{
					Tok tok=tokens[i];
					printf("%s",tok.str.buffer);
				}
				printf("\n");
			}

			break;
	}

	for(size_t i=0;i<pn->pnodes.size;++i)
		gen_x86_64(&((PNode*)pn->pnodes.buffer)[i]);
}
