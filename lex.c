#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include"lex.h"
#include"str.h"
#include"reg.h"
#include"err.h"
#include"state.h"

char*lextype_names[]={"LNONE","LIDENTIFIER","LINTEGER","LFLOAT","LSTRING","LOPERATOR","LKEYWORD","LCOMMENT","LMINUS",NULL};
char*lextype_colors[]={"\033[0m","\033[0m","\033[36m","\033[35m","\033[32m","\033[0m","\033[33m","\033[34m"};
char*lexsubtype_names[]={"LENDSTATEMENT","LASSIGN","LLPAREN","LRPAREN","LLCBRACE","LRCBRACE",NULL};
static char*operator_chars="-+*/=;(),.{}<>";
static char*keywords[]={"do","false","fn","for","if","let","ret","true","while","call",};
static char*operators[]={";","=","+=","-=","*=","/=","+","-","/","*","(",")","{","}"};

Lexer lex_new(void)
{
	Lexer l={
		.mode=LNONE,
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
	for(size_t i=0;i<strl+1;++i)
	{

		/*****
		 * initmatch(chset,mode,keepch)
		 * - Match initial character and change lexer to corresponding mode,
		 * - clear tmpstr and initialize new token
		 * chset        char*     set of characters which s[i] must match
		 * mode         uint32_t  change lexer mode to this
		 * keepch       bool      will we retain this character in the token string?
		 *****/
#define initmatch(chset,lmode,keepch) if(s[i]&&memchr((chset),s[i],strlen((chset)))){l->mode=(lmode);if(keepch)--i;vec_pushl(&l->tokens,((Tok){.str=str_new(),.type=l->mode,.line=current_line}));str_clear(&tmpstr);}

		/*****
		 * modeterminate
		 * - Finalize current token lexing and set state to LNONE
		 * keepch       bool      will we retain this character in the token string?
		 *****/
#define modeterminate(keepch) do{l->mode=LNONE;if(keepch)--i;str_assign(&((Tok*)l->tokens.buffer)[l->tokens.size-1].str,tmpstr.buffer);((Tok*)l->tokens.buffer)[l->tokens.size-1].line=current_line;ch[0]=s[i];str_append(&tmpstr,ch);}while(0)

		/*****
		 * modematch(chset,logic,keepch)
		 * - Match lexeme characters following initial character,
		 * - set token type and return lexer mode to normal
		 * chset        char*     set of characters which s[i] must match
		 * logic        bool      if false, only modify current token when s[i] does NOT match chset
		 * keepch       bool      will we retain this character in the token string?
		 *****/
#define modematch(chset,logic,keepch) do{if(!s[i]||(logic==(!!memchr((chset),s[i],strlen(chset)))) ){modeterminate(keepch);}ch[0]=s[i];str_append(&tmpstr,ch);}while(0)

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

			case LNONE:
				// Extra step: turn identifiers into keywords
				if(l->tokens.size)
				{
					Tok*lasttok=&((Tok*)l->tokens.buffer)[l->tokens.size-2];
					if(lasttok->type==LIDENTIFIER)
					{
						for(size_t j=0;j<sizeof(keywords)/sizeof(char*);++j)
							if(strcmp(keywords[j],lasttok->str.buffer)==0)
								lasttok->type=LKEYWORD;
					}
				}
				/* !! FALL THROUGH !! */
			default:

				// We go past the strl by one to
				// make sure the fixup stage (above)
				// always gets called
				if(i>=strl)break;

				initmatch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",LIDENTIFIER,true) else
				initmatch("\"",LSTRING,false) else
				initmatch("0123456789",LINTEGER,true) else
				initmatch("-",LMINUS,true) else
				initmatch(operator_chars,LOPERATOR,true) else
				initmatch("#",LCOMMENT,true) else
				if(strchr(" \t\n",s[i])){if(s[i]=='\n')++current_line;continue;} else
				err_log("%u: unrecognized character '%c' (%x)",current_line,((s[i]>32)?(s[i]):(' ')),s[i]);
				//initmatch(" \t\n",LNONE,false)
				break;

			// Individual modes
			case LIDENTIFIER:modematch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789",false,true);break;
			case LFLOAT:modematch("0123456789",false,true);break;
			case LINTEGER:modematch("0123456789.",false,true);
						 if(s[i]=='.')
						 {
							 ((Tok*)l->tokens.buffer)[l->tokens.size-1].type=LFLOAT;
							 l->mode=LFLOAT;
						 }
						 break;
			case LSTRING:modematch("\"",true,false);break;
			case LOPERATOR:modematch(operator_chars,false,true);
						   if(s[i]==';')
						   {
							   ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=LENDSTATEMENT;
							   modeterminate(false);
						   }
						   else if(s[i]=='='&&((Tok*)l->tokens.buffer)[l->tokens.size-1].str.size==1)
						   {
							   ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=LASSIGN;
						   }
						   else if(s[i]=='(')
						   {
							   ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=LLPAREN;
							   modeterminate(false);
						   }
						   else if(s[i]==')')
						   {
							   ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=LRPAREN;
							   modeterminate(false);
						   }
						   else if(s[i]=='{')
						   {
							   ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=LLCBRACE;
							   modeterminate(false);
						   }
						   else if(s[i]=='}')
						   {
							   ((Tok*)l->tokens.buffer)[l->tokens.size-1].subtype=LRCBRACE;
							   modeterminate(false);
						   }
						   break;
			case LMINUS://modematch("0123456789=",true,true);
						if(s[i]=='-')
						{
							ch[0]=s[i];str_append(&tmpstr,ch);
						}
						else if(strchr("0123456789",s[i]))
						{
							((Tok*)l->tokens.buffer)[l->tokens.size-1].type=LINTEGER;
							l->mode=LINTEGER;
							ch[0]=s[i];str_append(&tmpstr,ch);
						}
						else// if(s[i]=='=')
						{
							((Tok*)l->tokens.buffer)[l->tokens.size-1].type=LOPERATOR;
							l->mode=LOPERATOR;
							modeterminate(true);
						}
						break;
			case LCOMMENT:modematch("\n",true,true);break;

		}
#undef initmatch
#undef modematch
#undef modeterminate
	}

	str_free(&tmpstr);
	//reg_free(&regex);
}

void lex_print(Lexer*l)
{
	printf("%p: (%lu/%lu) [",l,l->tokens.size,l->tokens.capacity);
	for(size_t i=0;i<l->tokens.size;++i)
	{
		Tok*tok=&((Tok*)l->tokens.buffer)[i];
		printf("'%s'(%s %u)",
				tok->str.buffer,
				((tok->subtype)?(lexsubtype_names[tok->subtype-LENDSTATEMENT]):(lextype_names[tok->type])),
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
