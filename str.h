#pragma once

#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STRDEFSIZE 64

typedef struct Str
{
	char*buffer;
	size_t capacity;
	size_t size;
} Str;

Str str_new(void);
Str str_newa(char*c);
void str_append(Str*s,char*c);
void str_assign(Str*s,char*c);
Str str_basename(char*c);
void str_clear(Str*s);
void str_free(Str*s);
void str_grow(Str*s,size_t n);
void str_print(Str*s);
void str_randomize(Str*s);
void str_tr(Str*s,char a,char b);
