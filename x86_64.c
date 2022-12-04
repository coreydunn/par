#include<stdio.h>
#include<stdlib.h>
#include"x86_64.h"
#include"state.h"

/*TODO:
 * We have to handle child nodes within the switch
 * statement. This is so that prologs/epilogs can be
 * closed around dependent nodes appropriately.
 *****/
void gen_x86_64_prolog(PNode*pn,FILE*file,size_t stacksize);

void gen_x86_64(PNode*pn,FILE*file)
{
	size_t labelno=1;
	Tok*tokens=(Tok*)pn->tokens.buffer;

	if(!file)return;

	switch(pn->type)
	{

		case PFUNDECL:
			//gen_x86_64_prolog(pn,file,64);
			fprintf(file,"global %s\n%s:\n",tokens[0].str.buffer,tokens[0].str.buffer);
			break;

		case PVARDECL:
			if(pn->tokens.size>1)
			{
				if(tokens[1].type!=LOPERATOR||(strcmp(tokens[1].str.buffer,"=")))
					err_log("%u: expected either ';' or '='",tokens[1].line);
			}
			else if(pn->tokens.size==0)
				err_log("%u: expected identifier",pn->firstline);


			if(pn->tokens.size>2)
			{
				if(tokens[1].type==LOPERATOR&&tokens[1].str.buffer[0]=='=')
					fprintf(file,"\tmov eax,%s ;%s\n",tokens[2].str.buffer,tokens[0].str.buffer);
			}
			else if(pn->tokens.size>0)
			{
				fprintf(file,"\txor eax,eax ;%s\n",tokens[0].str.buffer);
			}
			break;

		case PRET:
				if(pn->tokens.size>0)
				{
					if(tokens[0].type!=LINTEGER)
						err_log("%u: returning non-integer",tokens[0].line);
					else
						fprintf(file,"\tmov eax,%s\n",tokens[0].str.buffer);
				}
				fprintf(file,"\tret\n");
			break;

		case PIF:
				if(pn->tokens.size>1)
					fprintf(file,"\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				fprintf(file,"\tjz .L%02lu\n",++labelno);
				fprintf(file,".L%02lu\n",labelno);
			break;

		case PWHILE:
				if(pn->tokens.size>1)
					fprintf(file,"\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				fprintf(file,"\tjz .L%02lu\n",++labelno);
				fprintf(file,".L%02lu\n",labelno);
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

void gen_x86_64_prolog(PNode*pn,FILE*file,size_t stacksize)
{
	if(!pn)
	{
		err_log("NULL PNode passed to gen_x86_64_prolog");
		return;
	}

	if(!file)
	{
		err_log("NULL FILE* passed to gen_x86_64_prolog");
		return;
	}

	fprintf(file,"push rbp\n");
	fprintf(file,"mov rbp,rsp\n");
	fprintf(file,"sub rsp,%lu\n",stacksize);
}
