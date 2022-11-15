#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"vec.h"
#include"reg.h"

void reg_match(Reg*r,char*p,char*s)
{
	//char*p="\\d";
	//char*s="1__23";
	//Vec r->matches=vec_new(sizeof(Match));

	// Iterate through pattern
	for(size_t j=0;p[j];++j)
	{

		//printf("((%lu))\n",j);
		if(strcmp(p+j,"\\d")==0)
		{
			++j;
			for(size_t i=0;s[i];++i)
				if(isdigit(s[i]))
				{
					Match m={.length=1,.str=s+i};
					vec_push(&r->matches,&m);
				}
		}
	}
}

void reg_match2(Reg*r,char*p,char*s)
{
	// Return if pointers are NULL
	if(!r||!p||!s)
		return;

	const size_t string_len=strlen(s);
	const size_t pat_len=strlen(p);
	bool found=false;
	bool done=false;
	size_t pat_idx=0;
	size_t string_idx=0;

	if(!pat_len||!string_len)
		return;

	//puts("begin");
	while(!done)
	{
		//printf("sz=%lu (%lu/%lu)\n",r->matches.unitsize,r->matches.size,r->matches.capacity);
		if(pat_idx>=pat_len||string_idx>=string_len)
			break;

		// Check for magic chars
		if(p[pat_idx]=='\\')
		{
			//puts("magic");
			++pat_idx;

			// Digit
			if(p[pat_idx]=='d' )
			{
				if(isdigit(s[string_idx]))
				{
					if(!found)
					{
						found=true;
						Match mm={.length=1,
							.str=s+string_idx};
						vec_push(&(r->matches),&mm);
					}

					else
					{
						((Match*)r->matches.buffer)[r->matches.size-1]
							.length++;
					}
				}

				else
					vec_pop(&r->matches);
			}

			// Alphabetical
			else if(p[pat_idx]=='c')
			{

				if(isalpha(s[string_idx]))
				{
					if(!found)
					{
						found=true;
						Match mm={.length=1,
							.str=s+string_idx};
						vec_push(&(r->matches),&mm);
					}

					else
					{
						((Match*)r->matches.buffer)[r->matches.size-1]
							.length++;
					}
				}

				else
				{
					if(found)
						vec_pop(&r->matches);
				}
			}
		}

		// NOMAGIC
		else
		{
			if(s[string_idx]==p[pat_idx])
				((Match*)r->matches.buffer)[r->matches.size-1]
					.length++;
		}

		++pat_idx;
		++string_idx;
	}
	//puts("done");
}

void reg_print(Reg*r)
{
	if(!r)return;
	printf("%p [",&r->matches);
	for(size_t i=0;i<r->matches.size;++i)
	{
		Match*m=((Match*)r->matches.buffer)+i;
		//char*string=m->str;

		printf("(%lu'",m->length);
		for(size_t j=0;j<m->length;++j)
			printf("%c",m->str[j]);
		printf("')");
		if(i<r->matches.size-1)
			printf(", ");
	}
	printf("]\n");
}

Reg reg_new(void)
{
	Reg r={.mode=MAGIC,.matches=vec_new(sizeof(Match))};
	return r;
}

void reg_free(Reg*r)
{
	if(!r)return;
	vec_free(&r->matches);
}
