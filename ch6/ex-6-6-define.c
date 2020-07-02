/*
 * Exercise 6-6
 *
 * Implement a simple version of the #define processor (i.e., no arguments)
 * suitable for use with C programs, based on the routines of this section.  You
 * may also find getch and ungetch helpful.
 */

#include <stdio.h>
#include "filter-code.h"
#include "getword.h"

#define MAXWORD 100

int main(void)
{
	struct charinfo *ci;
	while ((ci = getparsed()) != NULL)
		printf("%c", ci->ch);
	return 0;
}
