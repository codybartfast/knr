/*
 * Exercise 7-2
 *
 * Write a program that will print arbitrary input in a sensible way.  as a
 * minimum, it should print non-graphic characters in octal or hexadecimal
 * according to local custom, and break long text lines.
 */

#include <limits.h>
#include <stdio.h>
#include "bsbuff.h"
#include "nongraphic.h"
#include "detab.h"
#include "linesplit.h"

int tabsize = 6;
int linelen = 76;

int main(void)
{
	int c;
	int (*bsbuffed)(void), (*replaced)(void), (*linessplit)(void);

	configure_tabs(tabsize);

	bsbuffed = backspace_buffer(&getchar);
	replaced = replace_nongraphic(bsbuffed);
	linessplit = split_lines(replaced, linelen);

	while ((c = (*linessplit)()) != EOF)
		putchar(c);
	return 0;
}
