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
void gen_x86_64_epilog(PNode*pn,FILE*file);

void gen_x86_64(PNode*pn,FILE*file)
{
	size_t labelno=1;
	Tok*tokens=(Tok*)pn->tokens.buffer;

	if(!file)return;

	switch(pn->type)
	{

		case PFUNDECL:
			fprintf(file,"global %s\n%s:\n",tokens[0].str.buffer,tokens[0].str.buffer);
			gen_x86_64_prolog(pn,file,0);
			for(size_t i=0;i<pn->pnodes.size;++i)
				gen_x86_64(&((PNode*)pn->pnodes.buffer)[i],file);
			//gen_x86_64_epilog(pn,file);
			break;

		case PVARDECL:
			if(pn->tokens.size>1)
			{
				if(tokens[1].type!=LOPERATOR||(strcmp(tokens[1].str.buffer,"=")))
					err_log("%u: expected either ';' or '='",tokens[1].line);
			}
			else if(pn->tokens.size==0)
				err_log("%u: expected identifier",pn->firstline);

			fprintf(file,"\tsub rsp,%lu\n",4lu);
			if(pn->tokens.size>2)
			{
				if(tokens[1].type==LOPERATOR&&tokens[1].str.buffer[0]=='=')
					fprintf(file,"\tmov dword[rsp],%s ;%s\n",tokens[2].str.buffer,tokens[0].str.buffer);
			}
			else if(pn->tokens.size>0)
			{
				fprintf(file,"\txor eax,eax ;%s\n",tokens[0].str.buffer);
			}
			break;

		case PRET:
				gen_x86_64_epilog(pn,file);
				if(pn->tokens.size>0)
				{
					if(tokens[0].type!=LINTEGER)
						err_log("%u: returning non-integer",tokens[0].line);
					else
						fprintf(file,"\tmov eax,%s\n",tokens[0].str.buffer);
				}
				fprintf(file,"\tret\n");
				/*****
				 * IMPORTANT:
				 * This return exits the (recursive)
				 * code generation call and ceases
				 * reading further PNodes belonging
				 * to its parent node (i.e., on the
				 * same level)!!
				 *****/
				return;
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

		case PBLOCK:
			for(size_t i=0;i<pn->pnodes.size;++i)
				gen_x86_64(&((PNode*)pn->pnodes.buffer)[i],file);
			break;

		case PEMPTY:
			// Root node, otherwise ignore
			if(pn->parentnode==NULL)
			{
				for(size_t i=0;i<pn->pnodes.size;++i)
					gen_x86_64(&((PNode*)pn->pnodes.buffer)[i],file);
			}
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

	//for(size_t i=0;i<pn->pnodes.size;++i)
		//gen_x86_64(&((PNode*)pn->pnodes.buffer)[i],file);
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

	fprintf(file,"\tpush rbp ; prolog\n");
	fprintf(file,"\tmov rbp,rsp\n");
	fprintf(file,"\tsub rsp,%lu\n",stacksize);
}

void gen_x86_64_epilog(PNode*pn,FILE*file)
{
	if(!pn)
	{
		err_log("NULL PNode passed to gen_x86_64_epilog");
		return;
	}

	if(!file)
	{
		err_log("NULL FILE* passed to gen_x86_64_epilog");
		return;
	}

	fprintf(file,"\tmov rsp,rbp ; epilog\n");
	fprintf(file,"\tpop rbp\n");

}
