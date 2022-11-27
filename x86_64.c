#include<stdio.h>
#include<stdlib.h>
#include"x86_64.h"
#include"state.h"

void gen_x86_64(PNode*pn,FILE*file)
{
	Tok*tokens=(Tok*)pn->tokens.buffer;

	if(!file)return;

	switch(pn->type)
	{

		case PFUNDECL:
			fprintf(file,"global %s\n%s:\n",tokens[0].str.buffer,tokens[0].str.buffer);
			break;

		case PVARDECL:
			if(pn->tokens.size>2)
			{
				if(tokens[1].str.buffer[0]=='=')
					fprintf(file,"\tmov eax,%s ;%s\n",tokens[2].str.buffer,tokens[0].str.buffer);
			}
			else
				fprintf(file,"\txor eax,eax ;%s\n",tokens[0].str.buffer);
			break;

		case PRET:
				if(pn->tokens.size>1)
					fprintf(file,"\tmov eax,0 ;%s\n",tokens[1].str.buffer);
				fprintf(file,"\tret\n");
			break;

		case PIF:
				// if x == 7
				if(pn->tokens.size>1)
					fprintf(file,"\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				fprintf(file,"\tjz .L01\n");
			break;

		case PWHILE:
				if(pn->tokens.size>1)
					fprintf(file,"\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				fprintf(file,"\tjz .L01\n");
			break;

		case PCOMMENT:
				fprintf(file,"\t;%s\n",tokens[0].str.buffer);
			break;

		default:
			if(pn->tokens.size>0)
			{
				fprintf(file,";PNODE ID: %s ",partype_names[pn->type]);
				for(size_t i=0;i<pn->tokens.size;++i)
				{
					Tok tok=tokens[i];
					fprintf(file,"%s",tok.str.buffer);
				}
				fprintf(file,"\n");
			}

			break;
	}

	for(size_t i=0;i<pn->pnodes.size;++i)
		gen_x86_64(&((PNode*)pn->pnodes.buffer)[i],file);
}
