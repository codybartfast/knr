/*
 * Exercise 7-2
 *
 * Write a program that will print arbitrary input in a sensible way.  as a
 * minimum, it should print non-graphic characters in octal or hexadecimal
 * according to local custom, and break long text lines.
 */

#include <stdio.h>
#include "bsbuff.h"
#include "nongraphic.h"
#include "linesplit.h"
#include "align.h"
#include "detab.h"

int tabsize = 4;
int linelen = 70;
int maxrollover = 7;

int main(void)
{
	int c, (*getch)(void);

	configure_tabs(tabsize);

	getch = backspace_buffer(&getchar);
	getch = replace_nongraphic(getch);
	getch = split_lines(getch, linelen, maxrollover);
	getch = align(getch, linelen, maxrollover, ALIGN_JUSTIFIED);
	getch = detab(getch, linelen);

	while ((c = (*getch)()) != EOF)
		putchar(c);

	return 0;
}
