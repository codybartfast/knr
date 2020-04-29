/* Exercise 5-11
 *
 * Modify the programs entab and detab (written as exercises in Chapter 1)
 * to accept a list of tab stops as arguments.  Use the default tab settings
 * if there are no arguments.
 */

#include <stdio.h>
#include "tabstops.h"

int main(int argc, char *argv[])
{
	int col, c, dist;

	parsestops(argc, argv);

	col = 0;
	while ((c = getchar()) != EOF) {
		if (c == '\t') {
			for (dist = dist2stop(col); dist > 0; --dist) {
				putchar(' ');
				++col;
			}
		} else {
			putchar(c);
			if (c == '\n')
				col = 0;
			else if (c == '\b' && col > 0)
				--col;
			else
				++col;
		}
	}
	return 0;
}


/* Sample output

ch5> cat detab.txt | a.out 1 5 9 17 33 65
t0  t1  t2      t3              t4                              t5  t6  t7  t8

ch5>
*/
