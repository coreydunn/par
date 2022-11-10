#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include"lex.h"
#include"str.h"

Lexer lex_new(void)
{
	Lexer l={
		.mode=NONE,
		.lexemes=vec_new(sizeof(Lexeme)),
	};

	return l;
}

void lex_free(Lexer*l)
{
	if(!l)return;
	for(size_t i=0;i<l->lexemes.size;++i)
		str_free(&((Lexeme*)l->lexemes.buffer)[i].str);
	vec_free(&(l->lexemes));
}

// Read string and store lexemes
void lex_string(Lexer*l,char*s)
{
	Str tmp=str_new();
	char t[2]={0};
	size_t strl;

	if(!l||!s)return;

	//vec_pushl(&l->lexemes,((Lexeme){
			//.str=str_new(),
			//.type=NONE})
			//);

	strl=strlen(s);
	if(strl>1)++strl;

	// Read each individual byte
	for(size_t i=0;i<strl;++i)
	{

		switch(l->mode)
		{

			// New lexeme boundary
			// Starting condition
			/*****
			 * When we find a new lexeme,
			 * push a new Lexeme to the vector
			 * and set its type to the Lexer
			 * mode and initialize its str
			 *****/

			case NONE:
			default:
				if(isalpha(s[i])||s[i]=='_')
				{
					l->mode=IDENTIFIER;
					--i;
					vec_pushl(&l->lexemes,((Lexeme){.str=str_new(),.type=l->mode}));
					str_clear(&tmp);
				}

				else if(s[i]=='"')
				{
					l->mode=STRING;
					//--i;
					vec_pushl(&l->lexemes,((Lexeme){.str=str_new(),.type=l->mode}));
					str_clear(&tmp);
				}

				else if(isdigit(s[i]))
				{
					l->mode=INTEGER;
					--i;
					vec_pushl(&l->lexemes,((Lexeme){.str=str_new(),.type=l->mode}));
					str_clear(&tmp);
					//t[0]=s[i];str_append(&tmp,t);
				}

				else if(s[i]&&strchr("+=",s[i]))
				{
					l->mode=OPERATOR;
					--i;
					// not copying the operator string
					vec_pushl(&l->lexemes,((Lexeme){.str=str_new(),.type=l->mode}));
					str_clear(&tmp);
					//t[0]=s[i];str_append(&tmp,t);
					//str_assign(&((Lexeme*)l->lexemes.buffer)[l->lexemes.size-1].str,tmp.buffer);
					//l->mode=NONE;
				}
				break;

				// Individual modes
			case IDENTIFIER:
				if(!s[i]||(!isalnum(s[i])&&s[i]!='_'))
				{
					l->mode=NONE;
					--i;
					str_assign(&((Lexeme*)l->lexemes.buffer)[l->lexemes.size-1].str,tmp.buffer);
				}t[0]=s[i];str_append(&tmp,t);
				break;

			case INTEGER:
				if(!s[i]||!isdigit(s[i]))
				{
					l->mode=NONE;
					--i;
					str_assign(&((Lexeme*)l->lexemes.buffer)[l->lexemes.size-1].str,tmp.buffer);
				}t[0]=s[i];str_append(&tmp,t);
				break;

			case STRING:
				if(!s[i]||s[i]=='"')
				{
					l->mode=NONE;
					--i;
					str_assign(&((Lexeme*)l->lexemes.buffer)[l->lexemes.size-1].str,tmp.buffer);
				}t[0]=s[i];str_append(&tmp,t);
				break;

			case OPERATOR:
				if(!s[i]||!strchr("+=",s[i]))
				{
					l->mode=NONE;
					--i;
					str_assign(&((Lexeme*)l->lexemes.buffer)[l->lexemes.size-1].str,tmp.buffer);
				}t[0]=s[i];str_append(&tmp,t);
				break;

		}
	}

	str_free(&tmp);
}

void lex_print(Lexer*l)
{
	printf("%p: [",l);
	for(size_t i=0;i<l->lexemes.size;++i)
	{
		printf("'%s'(%u)",
				((Lexeme*)l->lexemes.buffer)[i].str.buffer,
				((Lexeme*)l->lexemes.buffer)[i].type
				);
		if(i<l->lexemes.size-1)
			printf(", ");
	}
	printf("]\n");
}
