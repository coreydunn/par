#include<stdio.h>
#include<stdarg.h>
#include"err.h"
#include"vec.h"
#include"str.h"

Err err_new(void)
{
	Err e={
		.msg=str_new(),
		.id=0,
	};
	return e;
}

void err_free(Err*e)
{
	if(!e)
	{
		fprintf(stderr,"error: invalid Err struct passed to err_free\n");
		return;
	}

	if(e->msg.buffer)
		str_free(&e->msg);
}

void err_print_vec(Vec*v)
{
	for(size_t i=0;i<v->size;++i)
	{
		Err*e=&((Err*)v->buffer)[i];
		fprintf(stderr,"%serror%s: %s (ERROR %u)\n",
				"\033[31m",
				"\033[0m",
				e->msg.buffer,
				e->id
				);
	}
}

void err_push(Vec*v,char*msg,uint32_t id)
{
	Err e=err_new();

	if(!v)
	{
		fprintf(stderr,"%serror%s: NULL pointer passed to err_push\n",
				"\033[34m",
				"\033[0m");
		return;
	}

	str_assign(&e.msg,msg);
	e.id=id;
	vec_push(v,&e);
}

void err_log(Vec*v,char*fmt,...)
{
	Str str=str_new();
	va_list va;

	char*strptr;
	static char sbuffer[1024];
	uint32_t ibuffer;

	if(!v)return;
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
	err_push(v,str.buffer,7);
	str_free(&str);
}
