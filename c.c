#include<stdio.h>
#include"str.h"

int main(void)
{
	Str s=str_new();

	str_assign(&s,"start");
	str_print(&s);
	str_assign(&s,"kangaroo");
	str_print(&s);
	str_assign(&s,"crazy");
	str_print(&s);
	str_append(&s," people");
	str_print(&s);

	str_tr(&s,' ','+');
	str_print(&s);

	str_clear(&s);
	str_append(&s,"what");
	str_print(&s);

	str_free(&s);
}
