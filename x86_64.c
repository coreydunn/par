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
			printf("\txor eax,eax ;%s\n",tokens[0].str.buffer);
			break;

		case PRET:
				if(pn->tokens.size>1)
					printf("\tmov eax,0 ;%s\n",tokens[1].str.buffer);
				printf("\tret\n");
			break;

		case PIF:
				// if x == 7
				if(pn->tokens.size>1)
					printf("\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				printf("\tjz .L01\n");
			break;

		case PWHILE:
				if(pn->tokens.size>1)
					printf("\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				printf("\tjz .L01\n");
			break;

		case PCOMMENT:
				printf("\t;%s\n",tokens[0].str.buffer);
			break;

		default:
			if(pn->tokens.size>0)
			{
				printf(";PNODE ID: %s ",partype_names[pn->type]);
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
