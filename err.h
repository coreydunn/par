#pragma once

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"str.h"
#include"vec.h"

typedef struct Err
{
	Str msg;
	uint32_t id;
} Err;

Err err_new(void);
void err_free(Err*e);
void err_print_vec(Vec*v);
void err_push(Vec*v,char*msg,uint32_t id);
void err_log(Vec*v,char*fmt,...);
