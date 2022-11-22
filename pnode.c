#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"pnode.h"
#include"tok.h"
#include"var.h"

char*partype_names[]={"PNONE","PEMPTY","PEXPRESSION","PSTATEMENT","PASSIGNMENT","PIF","PCOMMENT","PBLOCK","PWHILE","PVARDECL",NULL};

Parser parser_new(void)
{
	Parser p={
		.root=pnode_new(),
		.mode=0,
		.variables=vec_new(sizeof(Var)),
	};
	return p;
}

void parser_free(Parser*p)
{
	pnode_free(&p->root);
	for(size_t i=0;i<p->variables.size;++i)
		var_free(&((Var*)p->variables.buffer)[i]);
	vec_free(&p->variables);
}

PNode pnode_new(void)
{
	PNode n={
		.pnodes=vec_new(sizeof(PNode)), // Empty Vec
		.tokens=vec_new(sizeof(Tok)), // Empty Vec
		.parentnode=NULL,
		.type=PEMPTY,
	};

	return n;
}

void pnode_free(PNode*n)
{
	if(!n)return;

	for(size_t i=0;i<n->pnodes.size;++i)
		pnode_free(((PNode*)n->pnodes.buffer)+i);

	if(n->pnodes.buffer)
		vec_free(&n->pnodes);

	if(n->tokens.buffer)
		vec_free(&n->tokens);
}

PNode*pnode_pushnode(PNode*n)
{
	PNode t=pnode_new();
	if(!n)return NULL;

	vec_push(&n->pnodes,&t);
	((PNode*)n->pnodes.buffer)[n->pnodes.size-1].parentnode=n;
	return &((PNode*)n->pnodes.buffer)[n->pnodes.size-1];
}

void pnode_print(PNode*n,size_t lvl)
{
	const bool usecolor=true;

	for(size_t i=0;i<lvl;++i)
		printf("    ");
	printf("%p: (c:%lu/%lu) (t:%lu/%lu) %lu:",
			n,
			n->pnodes.size,
			n->pnodes.capacity,
			n->tokens.size,
			n->tokens.capacity,
			(n->tokens.size>0)?(((Tok*)n->tokens.buffer)[0].line):(0)
		  );

	// Print tokens
	printf(" [");
	for(size_t i=0;i<n->tokens.size;++i)
	{
		if(usecolor)
			printf("'%s%s%s'",
					lextype_colors[((Tok*)n->tokens.buffer)[i].type],
					((Tok*)n->tokens.buffer)[i].str.buffer,
					"\033[0m"
					);
		else
			printf("'%s'",((Tok*)n->tokens.buffer)[i].str.buffer);
		if(i<n->tokens.size-1)
			printf(", ");
	}
	printf("]");

	printf(" %s\n",partype_names[n->type]);

	++lvl;

	if(n->pnodes.size>0)
	{
		for(size_t i=0;i<n->pnodes.size;++i)
			pnode_print(((PNode*)n->pnodes.buffer)+i,lvl);
	}
}

void pnode_print_brief(PNode*n,size_t lvl)
{
	const bool usecolor=true;

	for(size_t i=0;i<lvl;++i)
		printf("    ");

	if(n->tokens.size>0)
		printf("%lu: ",((Tok*)n->tokens.buffer)[0].line);
	printf("%s: ",partype_names[n->type]);

	// Print tokens
	for(size_t i=0;i<n->tokens.size;++i)
	{
		//printf("%s",((Tok*)n->tokens.buffer)[i].str.buffer);
		if(usecolor)
			printf("%s%s%s",
					lextype_colors[((Tok*)n->tokens.buffer)[i].type],
					((Tok*)n->tokens.buffer)[i].str.buffer,
					"\033[0m"
				  );
		else
			printf("%s",((Tok*)n->tokens.buffer)[i].str.buffer);
	}
	printf("\n");

	++lvl;

	if(n->pnodes.size>0)
		for(size_t i=0;i<n->pnodes.size;++i)
			pnode_print_brief(((PNode*)n->pnodes.buffer)+i,lvl);
}

// Parse Vec of Tok
void parser_parse(Parser*p,Vec*t)
{
	PNode*current_node=NULL;
	Tok*cur_tok=NULL;
	if(!p||!t)return;

	p->root.type=PEMPTY;

	for(size_t i=0;i<t->size;++i)
	{
		if(!current_node)current_node=&p->root;
		cur_tok=((Tok*)t->buffer)+i;
		//printf("%lu ",i);
#define descend(m) do{--i;current_node=pnode_pushnode(current_node);current_node->type=m;p->mode=m;}while(0)
#define pushcurrenttoken() do{vec_pushta(&current_node->tokens,cur_tok->str.buffer);tok_copy_nostr(&((Tok*)current_node->tokens.buffer)[current_node->tokens.size-1],&((Tok*)t->buffer)[i]);}while(0)
#define up() do{if(current_node&&current_node->parentnode)current_node=current_node->parentnode;}while(0)
#define checktype(t) if(cur_tok->type==t)
#define checktypesub(t,s) if(cur_tok->type==t&&cur_tok->subtype==s)
		switch(p->mode)
		{

			case PNONE:
				while(current_node->parentnode&&(current_node->parentnode->type==PIF||
						current_node->parentnode->type==PWHILE))
					up();
				up();
				switch(cur_tok->type)
				{

					case LKEYWORD:
						if(strcmp("if",cur_tok->str.buffer)==0){++i;descend(PIF);}
						else if(strcmp("while",cur_tok->str.buffer)==0){++i;descend(PWHILE);}
						else if(strcmp("let",cur_tok->str.buffer)==0){++i;descend(PVARDECL);}
						break;

					case LOPERATOR:
						if(cur_tok->subtype==LLCBRACE){p->mode=PBLOCK;descend(PBLOCK);}
						else if(cur_tok->subtype==LRCBRACE){up();}
						break;

					case LCOMMENT:descend(PCOMMENT);break;
					default:descend(PEXPRESSION);break;
				}
				break;

			case PCOMMENT:pushcurrenttoken();p->mode=PNONE;break;

			case PIF:
				pushcurrenttoken();
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LRPAREN){
					++i;p->mode=PNONE;descend(PEXPRESSION);break;}
				break;

			case PWHILE:
				pushcurrenttoken();
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LRPAREN){
					++i;p->mode=PNONE;descend(PEXPRESSION);break;}
				break;

			case PBLOCK:
				checktypesub(LOPERATOR,LRCBRACE){up();p->mode=PNONE;break;}
				descend(PEXPRESSION);
				break;

			case PVARDECL:
				checktypesub(LOPERATOR,LENDSTATEMENT){p->mode=PNONE;break;}
				pushcurrenttoken();
				break;

				// Default Grammar Rule:
				// TODO: The default case needs more sophisticated grammar rules
				// And we should also split cases apart
			case PEXPRESSION:
			case PASSIGNMENT:
			default:

				checktype(LCOMMENT){current_node->type=PCOMMENT;p->mode=PCOMMENT;--i;break;}
				else checktypesub(LOPERATOR,LENDSTATEMENT){
					if(current_node->parentnode->type==PIF)up();
					else if(current_node->parentnode->type==PWHILE)up();
					p->mode=PNONE;break;
				}
				else checktypesub(LOPERATOR,LASSIGN){p->mode=PASSIGNMENT;current_node->type=PASSIGNMENT;}
				else checktypesub(LOPERATOR,LLCBRACE){p->mode=PBLOCK;current_node->type=PBLOCK;break;}
				else checktypesub(LOPERATOR,LRCBRACE){p->mode=PNONE;up();break;}
				else if(cur_tok->type==LKEYWORD){
					if(strcmp("if",cur_tok->str.buffer)==0){p->mode=PIF;current_node->type=PIF;break;}
					else if(strcmp("while",cur_tok->str.buffer)==0){p->mode=PWHILE;current_node->type=PWHILE;break;}
				}
				pushcurrenttoken();
				//vec_pushta(&current_node->tokens,);
				break;
#undef checktype
#undef checktypesub

		}
#undef descend
#undef pushcurrenttoken
#undef up
	}
}
