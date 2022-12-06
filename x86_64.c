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

	if(!pn)
	{
		err_log("passed NULL PNode to gen_x86_64");
		return;
	}

	if(!file)
	{
		err_log("NULL FILE* passed to gen_x86_64");
		return;
	}


	switch(pn->type)
	{

		case PFUNDECL:
			if(pn->parentnode&&pn->parentnode->parentnode!=NULL)
				err_log("%u: nested function declaration",pn->firstline);
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
					if(tokens[0].type==LOPERATOR&&tokens[0].subtype==LSMINUS)
					{
						if(pn->tokens.size<2)
							err_log("%u: expected integer",tokens[0].line);
						else
							fprintf(file,"\tmov eax,-%s\n",tokens[1].str.buffer);
					}
					else if(tokens[0].type!=LINTEGER)
					{
						err_log("%u: returning non-integer",tokens[0].line);
					}
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

		case PCALL:
			if(pn->tokens.size<1)
			{
				err_log("%u: expected function name after 'call'",tokens[0].line);
				break;
			}
			fprintf(file,"\tcall %s\n",tokens[0].str.buffer);
			break;

		case PIF:
				if(pn->tokens.size>1)
					fprintf(file,"\tcmp eax,0 ;%s\n",tokens[1].str.buffer);
				fprintf(file,"\tjz .L%02lu\n",++labelno);
				fprintf(file,".L%02lu:\n",labelno);
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

		case PEXPRESSION:
		default:
			gen_x86_64_eval(pn,file);
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

// Evaluate expression using RPN algorithm
void gen_x86_64_eval2(PNode*pn,FILE*file)
{
	Vec ints=vec_new(sizeof(Tok*));
	Vec ops=vec_new(sizeof(Tok*));
	Tok*tokens=NULL;

	if(!pn)
	{
		err_log("passed NULL PNode to gen_x86_64_eval");
		return;
	}

	if(!file)
	{
		err_log("NULL FILE* passed to gen_x86_64_eval");
		return;
	}

	tokens=((Tok*)pn->tokens.buffer);

	//printf("gen_x86_64_eval: %p [",tokens);
	for(size_t i=0;i<pn->tokens.size;++i)
	{
		//printf("'%s' ",tokens[i].str.buffer);

		switch(tokens[i].type)
		{
			case LIDENTIFIER:
			case LINTEGER:vec_push(&ints,tokens+i);break;
			case LOPERATOR:
				vec_push(&ops,tokens+i);
				break;
			  //{
				  //while(ops.size>0)
				  //{
//
					  //// Perform evaluation of 2 ints and
					  //// 1 operator
					  //for(size_t j=0;j<ops.size;++j)
					  //{
						  //Tok*x=NULL,*y=NULL;
//
						  //// Pop 2 ints
						  //x=&((Tok*)ints.buffer)[ints.size-1];
						  //vec_pop(&ints);
						  //y=&((Tok*)ints.buffer)[ints.size-1];
						  //vec_pop(&ints);
//
						  //printf("gen_x86_64_eval: '%s' %s '%s'\n",
								  //x->str.buffer,
								  //((Tok*)ops.buffer)[j].str.buffer,
								  //y->str.buffer
								  //);
//
						  //vec_pop(&ops);
//
					  //}
				  //}
				  //vec_push(&ops,tokens+i);
				  //break;
			  //}
			default:break;
		}

	}
	//printf("]\n");

	// Print vectors
	printf("ints: ");
	for(size_t i=0;i<ints.size;++i)
		printf("'%s'(%s), ",
				((Tok*)ints.buffer)[i].str.buffer,
				lextype_names[((Tok*)ints.buffer)[i].type]
				);
	printf("\n");

	printf("ops: ");
	for(size_t i=0;i<ops.size;++i)
		printf("'%s'(%s), ",
				((Tok*)ops.buffer)[i].str.buffer,
				lextype_names[((Tok*)ops.buffer)[i].type]
				);
	printf("\n");

	vec_free(&ints);
	vec_free(&ops);
}

void gen_x86_64_eval(PNode*pn,FILE*file)
{
	Vec ints=vec_new(sizeof(Tok));
	Vec ops=vec_new(sizeof(Tok));
	Tok*tokens=NULL;

	if(!pn)
	{
		err_log("passed NULL PNode to gen_x86_64_eval");
		return;
	}

	if(!file)
	{
		err_log("NULL FILE* passed to gen_x86_64_eval");
		return;
	}

	tokens=((Tok*)pn->tokens.buffer);

	for(size_t i=0;i<pn->tokens.size;++i)
	{
		switch(tokens[i].type)
		{

			case LINTEGER:
			case LIDENTIFIER:
				{
					Tok tmp=tokens[i];
					vec_push(&ints,&tmp);
				}
				break;

			case LOPERATOR:
				{
					Tok tmp=tokens[i];
					vec_push(&ops,&tmp);
				}
				break;
			default:
				break;

		}
	}

	printf("ints: [");
	for(size_t i=0;i<ints.size;++i)
	{
		printf("'%s'",((Tok*)ints.buffer)[i].str.buffer);
		if(i<ints.size-1)
			printf(", ");
	}
	printf("]\n");

	printf("ops: %p [",&ops);
	for(size_t i=0;i<ops.size;++i)
	{
		printf("'%s'",((Tok*)ops.buffer)[i].str.buffer);
		if(i<ops.size-1)
			printf(", ");
	}
	printf("]\n");

	// Math
	while(ops.size>0)
	{
		Tok op=((Tok*)ops.buffer)[ops.size-1];
		Tok t=tok_new();

		const Tok i2=((Tok*)ints.buffer)[ints.size-1];
		vec_pop(&ints);
		const Tok i1=((Tok*)ints.buffer)[ints.size-1];
		vec_pop(&ints);

		{
			Tok tmp={.type=LFAKE};
			vec_push(&ints,&tmp);
		}

		printf("%s %s %s\n",op.str.buffer,
				i1.str.buffer,
				i2.str.buffer);

		vec_pop(&ops);
		tok_free(&t);
	}

	vec_free(&ints);
	vec_free(&ops);
}
