#include<stdio.h>
#include<stdlib.h>
#include"str.h"
#include"vec.h"
#include"pnode.h"
#include"tok.h"

char*partype_names[]={"EMPTY","EXPRESSION","STATEMENT","ASSIGNMENT","IFSTATEMENT","DECLARATION","COMMENT","CODEBLOCK"};

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
		.type=EMPTY,
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
	for(size_t i=0;i<lvl;++i)
		printf("    ");

	if(n->tokens.size>0)
		printf("%lu: ",((Tok*)n->tokens.buffer)[0].line);
	printf("%s: ",partype_names[n->type]);

	// Print tokens
	for(size_t i=0;i<n->tokens.size;++i)
		printf("%s",((Tok*)n->tokens.buffer)[i].str.buffer);
	printf("\n");

	++lvl;

	if(n->pnodes.size>0)
		for(size_t i=0;i<n->pnodes.size;++i)
			pnode_print_brief(((PNode*)n->pnodes.buffer)+i,lvl);
}

// Parse Vec of Tok
void parser_tokens(Parser*p,Vec*t)
{
	if(!p)return;
	if(!t)return;

	// Do not push tokens to root node
	PNode*current_node=pnode_pushnode(&p->root);

	// Copy tokens, create PNodes
	for(size_t i=0;i<t->size;++i)
	{

		/*****
		 * match(lex_type,lex_subtype,settype,newtype,endnode)
		 * lex_type     uint32_t  token must match this
		 * lex_subtype  uint32_t  token must match this
		 * settype      bool      will we modify node type?
		 * newtype      uint32_t  change current_node type to this
		 * endnode      bool      will we terminate current_node?
		 *****/
#define match(lex_type,lex_subtype,settype,newtype,endnode) if(src->type==lex_type&&src->subtype==lex_subtype){if(settype)current_node->type=newtype;if(endnode&&i<t->size-1)current_node=pnode_pushnode(&p->root);}
		Tok*src=&((Tok*)t->buffer)[i];

		// Push token
		//current_node->type=STATEMENT;
		vec_pushta(&current_node->tokens,src->str.buffer);
		tok_copy_nostr(&((Tok*)current_node->tokens.buffer)[current_node->tokens.size-1],src);
		if(current_node->type==EMPTY)
			current_node->type=EXPRESSION;

		// TODO: match assumes that the previous node has
		// been terminated. If not, it will modify the current
		// PNode based on the current token and start a new PNode.
		//
		// Problem: Initial matches will override the current PNode
		// with the new PNode type and tokens, combining them together
		//
		// Solution: ensure unexpected token types will terminate
		// previous PNodes, using grammar rules
		// Example: ASSIGNMENT --> [IDENTIFIER, ASSIGN, OPERATOR, INTEGER, FLOAT]
		//          COMMENT    --> [COMMENT] 
		// See pnode.h

		// Terminate statement node, set statement type
		match(OPERATOR,ENDSTATEMENT,false,STATEMENT,true) else
		match(OPERATOR,ASSIGN,true,ASSIGNMENT,false) else
		match(LCOMMENT,0,true,COMMENT,true)
#undef match
	}

}
