#include<stdio.h>
#include<stdarg.h>
#include"err.h"
#include"vec.h"
#include"str.h"

void err_log(char*fmt,...)
{
	Str str=str_new();
	va_list va;

	char*strptr;
	static char sbuffer[1024];
	uint32_t ibuffer;

	if(!fmt)return;
	va_start(va,fmt);

	for(size_t i=0;fmt[i];++i)
	{
		if(fmt[i]=='%')
		{
			++i;
			if(!fmt[i])break;
			switch(fmt[i])
			{

				case 'i':
				case 'd':
				case 'u':
					ibuffer=va_arg(va,uint32_t);
					sprintf(sbuffer,"%u",ibuffer);
					str_append(&str,sbuffer);
					break;

				case 'x':
					ibuffer=va_arg(va,uint32_t);
					sprintf(sbuffer,"%#x",ibuffer);
					str_append(&str,sbuffer);
					break;

				case 'c':
					sbuffer[0]=va_arg(va,int);
					sbuffer[1]=0;
					str_append(&str,sbuffer);
					break;

				case 's':
					strptr=va_arg(va,char*);
					str_append(&str,strptr);
					break;

				default:
					break;
			}
		}

		else
		{
			sbuffer[0]=fmt[i];
			sbuffer[1]=0;
			str_append(&str,sbuffer);
		}
	}

	va_end(va);

	fprintf(stderr,"%serror%s: %s\n",
			"\033[31m",
			"\033[0m",
			str.buffer
			);

	str_free(&str);
}
