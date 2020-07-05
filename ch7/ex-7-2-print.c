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
 *      ------->########<--------
 * 
 */

#include <stdio.h>
#include "bsbuff.h"

int main(void)
{
	int c, (*bsbuffer)(void) = backspace_buffer(&getchar);
	while((c = (*bsbuffer)()) != EOF)
		putchar(c);
	return 0;
}
