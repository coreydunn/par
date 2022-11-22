#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"pnode.h"
#include"tok.h"

char*partype_names[]={"PNONE","PEMPTY","PEXPRESSION","PSTATEMENT","PASSIGNMENT","PIFSTATEMENT","PDECLARATION","PCOMMENT","PBLOCK","PWHILESTATEMENT"};

Parser parser_new(void)
{
	Parser p={
		.root=pnode_new(),
		.mode=0,
	};
	return p;
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
		switch(p->mode)
		{

			case PNONE:
				while(current_node->parentnode&&(current_node->parentnode->type==PIFSTATEMENT||
						current_node->parentnode->type==PWHILESTATEMENT))
					up();
				up();
				switch(cur_tok->type)
				{

					case LKEYWORD:
						if(strcmp("if",cur_tok->str.buffer)==0){++i;descend(PIFSTATEMENT);}
						else if(strcmp("while",cur_tok->str.buffer)==0){++i;descend(PWHILESTATEMENT);}
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

			case PIFSTATEMENT:
				pushcurrenttoken();
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LRPAREN){
					++i;p->mode=PNONE;descend(PEXPRESSION);break;}
				break;

			case PWHILESTATEMENT:
				pushcurrenttoken();
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LRPAREN){
					++i;p->mode=PNONE;descend(PEXPRESSION);break;}
				break;

			case PBLOCK:
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LRCBRACE){up();p->mode=PNONE;break;}
				descend(PEXPRESSION);
				break;

				// TODO: The default case needs more sophisticated grammar rules
				// And we should also split cases apart
			case PEXPRESSION:
			case PASSIGNMENT:
			default:
				if(cur_tok->type==LCOMMENT){p->mode=PCOMMENT;--i;break;}
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LENDSTATEMENT){if(current_node->parentnode->type==PIFSTATEMENT)up();
					p->mode=PNONE;break;}
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LENDSTATEMENT){if(current_node->parentnode->type==PWHILESTATEMENT)up();
					p->mode=PNONE;break;}
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LASSIGN){p->mode=PASSIGNMENT;current_node->type=PASSIGNMENT;}
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LLCBRACE){p->mode=PBLOCK;current_node->type=PBLOCK;break;}
				if(cur_tok->type==LOPERATOR&&cur_tok->subtype==LRCBRACE){p->mode=PNONE;up();break;}
				if(cur_tok->type==LKEYWORD&&strcmp("if",cur_tok->str.buffer)==0){p->mode=PIFSTATEMENT;current_node->type=PIFSTATEMENT;break;}
				if(cur_tok->type==LKEYWORD&&strcmp("while",cur_tok->str.buffer)==0){p->mode=PWHILESTATEMENT;current_node->type=PWHILESTATEMENT;break;}
				pushcurrenttoken();
				//vec_pushta(&current_node->tokens,);
				break;
		}
#undef descend
#undef pushcurrenttoken
#undef up
	}
}
