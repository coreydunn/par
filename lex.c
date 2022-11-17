#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include"lex.h"
#include"str.h"
#include"reg.h"

char*lextype_names[]={"NONE","IDENTIFIER","INTEGER","FLOAT","STRING","OPERATOR","KEYWORD","LCOMMENT"};

Lexer lex_new(void)
{
	Lexer l={
		.mode=NONE,
		.tokens=vec_new(sizeof(Tok)),
	};

	return l;
}

void lex_free(Lexer*l)
{
	if(!l)return;
	for(size_t i=0;i<l->tokens.size;++i)
		str_free(&((Tok*)l->tokens.buffer)[i].str);
	vec_free(&(l->tokens));
}

// Read string and store tokens
void lex_string(Lexer*l,char*s)
{
	Reg regex=reg_new();
	Str tmp=str_new();
	char t[2]={0};
	size_t strl;
	char*operators="-+*/=;(),";
	char*keywords[]={"for","if","while","do","true","false"};
	size_t current_line=1;

	if(!l||!s)return;

	strl=strlen(s);
	//if(strl>1)++strl;

	// Read each individual byte
	for(size_t i=0;i<strl;++i)
	{

#define initmatch(set,x,stripchar) if(s[i]&&memchr((set),s[i],strlen((set)))){l->mode=(x);if(stripchar)--i;vec_pushl(&l->tokens,((Tok){.str=str_new(),.type=l->mode,.line=current_line}));str_clear(&tmp);}
#define modematch(set,logic,stripchar) do{if(!s[i]||(logic==(!!memchr((set),s[i],strlen(set)))) ){l->mode=NONE;if(stripchar)--i;str_assign(&((Tok*)l->tokens.buffer)[l->tokens.size-1].str,tmp.buffer);((Tok*)l->tokens.buffer)[l->tokens.size-1].line=current_line;}t[0]=s[i];str_append(&tmp,t);}while(0)
		switch(l->mode)
		{

			// New lexeme boundary
			// Starting condition
			/*****
			 * When we find a new lexeme,
			 * push a new Tok to the vector
			 * and set its type to the Lexer
			 * mode and initialize its str
			 *****/
			case NONE:
			default:
				initmatch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",IDENTIFIER,true) else
				initmatch("\"",STRING,false) else
				initmatch("0123456789",INTEGER,true) else
				initmatch(operators,OPERATOR,true) else
				initmatch("#",LCOMMENT,true) else
				if(strchr(" \t\n",s[i])){if(s[i]=='\n')++current_line;continue;} else
				fprintf(stderr,"error: %lu: unrecognized character '%c' (%x)\n",current_line,((s[i]>32)?(s[i]):(' ')),s[i]);
				//initmatch(" \t\n",NONE,false)
				break;

				/****
				 * TODO: something is wrong with modematch
				 * which puts garbage chars at the beginning
				 * of tmp
				 ****/
			// Individual modes
			case IDENTIFIER:modematch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789",false,true);
							for(size_t j=0;j<sizeof(keywords)/sizeof(char*);++j)
								if(strcmp(keywords[j],tmp.buffer)==0)
									((Tok*)l->tokens.buffer)[l->tokens.size-1].type=KEYWORD;
							break;
			case INTEGER:modematch("0123456789.",false,true);
						 {
							 size_t n=lex_strchrcount(tmp.buffer,'.');
							 if(n==1)
								 ((Tok*)l->tokens.buffer)[l->tokens.size-1].type=FLOAT;
							 else if(n>1)
								 fprintf(stderr,"error: %lu: invalid float format '%s'\n",current_line,tmp.buffer);
						 }
						 break;
			case STRING:modematch("\"",true,false);break;
			case OPERATOR:modematch(operators,false,true);break;
			case LCOMMENT:modematch("\n",true,true);break;

		}
#undef initmatch
#undef modematch
	}

	str_free(&tmp);
	reg_free(&regex);
}

void lex_print(Lexer*l)
{
	printf("%p: [",l);
	for(size_t i=0;i<l->tokens.size;++i)
	{
		printf("'%s'(%s %u)",
		//printf("'%s'(%s)",
				((Tok*)l->tokens.buffer)[i].str.buffer,
				lextype_names[((Tok*)l->tokens.buffer)[i].type],
				((Tok*)l->tokens.buffer)[i].type
				);
		if(i<l->tokens.size-1)
			printf(", ");
	}
	printf("]\n");
}

size_t lex_strchrcount(char*str,char c)
{
	size_t count=0;
	for(size_t i=0;str[i];++i)
		if(str[i]==c)
			++count;
	return count;
}
