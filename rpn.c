#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>

#define VSIZE 256

struct vec{int b[VSIZE];int n;};
struct state{struct vec nums;struct vec ops;};

int rpn_pop(struct vec*v);
void rpn_math(struct state*st);
void rpn_parse(struct state*st,char*string);
void rpn_print(struct vec*v,char*fmt);
void rpn_push(struct vec*v,int c);

//int main(int argc,char**argv)
//{
	//struct state st={0};
	//char*str="7-1+2";
//
	//if(argc>1)str=argv[1];
//
	//printf("str: '%s'\n",str);
//
	//parse(&st,str);
	//printf("parse:\n");
	//print(&(st.nums),"%d");
	//print(&(st.ops),"%c");
//
	//while(st.ops.n)math(&st);
	//printf("math:\n");
	//print(&(st.nums),"%d");
	//print(&(st.ops),"%c");
//}

void rpn_push(struct vec*v,int c){(v)->b[(v)->n]=(c);++(v)->n;}
int rpn_pop(struct vec*v){if((v)->n){--(v)->n;return v->b[v->n];}return -1;}

void rpn_parse(struct state*st,char*string)
{
	bool found=false;

	if(!st||!string)return;
	for(int i=0;string[i];++i)
	{
		if(isdigit(string[i]))
			push((&st->nums),string[i]-'0');
		else if(strchr("+-*/",string[i]))
		{
			while(st->ops.n>0)
				math(st);
			push((&st->ops),string[i]);
		}
	}
}

void rpn_math(struct state*st)
{
	if(!st)return;
	//printf("math: ");

	for(int i=0;i<st->ops.n;++i)
	{
		//printf("(%c ",st->ops.b[i]);
		int x=pop(&st->nums);
		int y=pop(&st->nums);
		int z;

		switch(st->ops.b[i])
		{
			case '+':z=y+x;break;
			case '-':z=y-x;break;
			case '*':z=y*x;break;
			case '/':z=y/x;break;
			default:break;
		}

		//printf("%d,%d): %d ",y,x,z);
		push(&st->nums,z);
		pop(&st->ops);
		//printf("N:");print(&st->nums,"%d");
	}
	//printf("\n");
}

void rpn_print(struct vec*v,char*fmt)
{
#define p printf
	p("(%d/%d) [",v->n,VSIZE);
	for(int i=0;i<v->n;++i)
	{
		p(fmt,v->b[i]);
		if(i<v->n-1)
			p(", ");
	}
	p("]\n");
#undef p
}
