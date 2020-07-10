#include <stdio.h>
#include "detab.h"

static int tsize = 4;

void configure_tabs(int tabsize){
	tsize = tabsize;
}

int nextstop(int col)
{
	return (((col + 1) / tsize) + 1) * tsize;
}
