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
#include "linesplit.h"
#include "align.h"
#include "detab.h"

static const int tabsize = 6;
static const int linelen = 76;
static const int maxrollover = 7;

int main(void)
{
	int c, (*get_char)(void);

	configure_tabs(tabsize);

	get_char = &getchar;
	get_char = backspace_buffer(get_char);
	get_char = replace_nongraphic(get_char);
	get_char = split_lines(get_char, linelen, maxrollover);
	get_char = align(get_char, linelen, maxrollover, ALIGN_CENTRE);
	get_char = detab(get_char, linelen);

	while ((c = (*get_char)()) != EOF)
		putchar(c);

	return 0;
}
