#pragma once

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"str.h"

typedef struct Err
{
	Str msg;
	uint32_t id;
} Err;

Err err_new(void);
void err_free(Err*e);
