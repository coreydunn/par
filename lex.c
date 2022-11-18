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
char*lexsubtype_names[]={"ENDSTATEMENT","ASSIGN"};
static char*operators="-+*/=;(),.";
static char*keywords[]={"for","if","while","do","true","false"};

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
	//Reg regex=reg_new();
	Str tmpstr=str_new();
	char ch[2]={0};
	size_t current_line=1;
	size_t strl;

	if(!l||!s)return;

	strl=strlen(s);

	// Read each individual byte
	for(size_t i=0;i<strl;++i)
	{

		/*****
		 * initmatch(chset,mode,keepch)
		 * - Match initial character and change lexer to corresponding mode,
		 * - clear tmpstr and initialize new token
		 * chset        char*     set of characters which s[i] must match
		 * mode         uint32_t  change lexer mode to this
		 * keepch    bool      will we retain this character in the token string?
		 *****/
#define initmatch(chset,lmode,keepch) if(s[i]&&memchr((chset),s[i],strlen((chset)))){l->mode=(lmode);if(keepch)--i;vec_pushl(&l->tokens,((Tok){.str=str_new(),.type=l->mode,.line=current_line}));str_clear(&tmpstr);}

		/*****
		 * modematch(chset,logic,keepch)
		 * - Match lexeme characters following initial character,
		 * - set token type and return lexer mode to normal
		 * chset        char*     set of characters which s[i] must match
		 * logic        bool      if false, only modify current token when s[i] does NOT match chset
		 * keepch       bool      will we retain this character in the token string?
		 *****/
#define modematch(chset,logic,keepch) do{if(!s[i]||(logic==(!!memchr((chset),s[i],strlen(chset)))) ){l->mode=NONE;if(keepch)--i;str_assign(&((Tok*)l->tokens.buffer)[l->tokens.size-1].str,tmpstr.buffer);((Tok*)l->tokens.buffer)[l->tokens.size-1].line=current_line;}ch[0]=s[i];str_append(&tmpstr,ch);}while(0)

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

			// Individual modes
			case IDENTIFIER:modematch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789",false,true);
							for(size_t j=0;j<sizeof(keywords)/sizeof(char*);++j)
								if(strcmp(keywords[j],tmpstr.buffer)==0)
									((Tok*)l->tokens.buffer)[l->tokens.size-1].type=KEYWORD;
							break;
			case FLOAT:modematch("0123456789",false,true);break;
			case INTEGER:modematch("0123456789.",false,true);
						 if(s[i]=='.')
						 {
							 ((Tok*)l->tokens.buffer)[l->tokens.size-1].type=FLOAT;
							 l->mode=FLOAT;
						 }
						 break;
			case STRING:modematch("\"",true,false);break;
			case OPERATOR:modematch(operators,false,true);
						  if(s[i]==';')
							  ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=ENDSTATEMENT;
						  else if(s[i]=='=')
							  ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=ASSIGN;
						  break;
			case LCOMMENT:modematch("\n",true,true);break;

		}
#undef initmatch
#undef modematch
	}

	str_free(&tmpstr);
	//reg_free(&regex);
}

void lex_print(Lexer*l)
{
	printf("%p: [",l);
	for(size_t i=0;i<l->tokens.size;++i)
	{
		Tok*tok=&((Tok*)l->tokens.buffer)[i];
		printf("'%s'(%s %u)",
				tok->str.buffer,
				((tok->subtype)?(lexsubtype_names[tok->subtype-ENDSTATEMENT]):(lextype_names[tok->type])),
				tok->type
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
