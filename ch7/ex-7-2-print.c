/*
 * Exercise 7-2
 *
 * Write a program that will print arbitrary input in a sensible way.  as a
 * minimum, it should print non-graphic characters in octal or hexadecimal
 * according to local custom, and break long text lines.
 */

/*
 * Backspace buffer
 * substitution
 * lines split (tabstop)
 * Justify
 * detab (tabstop)
 *
 *      ------->########<-------
 *      X><X
 */

#include <stdio.h>
#include "bsbuff.h"
#include "nongraphic.h"

#include "limits.h"

int main(void)
{
	int c;
	int (*bsbuffed)(void) = backspace_buffer(&getchar);
	int (*replaced)(void) = replace_nongraphic(bsbuffed);
	while ((c = (*replaced)()) != EOF)
		putchar(c);
	return 0;
}
