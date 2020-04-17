/*
 * Exercise 4-14
 * 
 * Define a macro swap(t, x, y) that interchanges two arguments of type t. (
 * Block structure will help.)
 */

#include <stdio.h>

#define swap(t, x, y)                                                          \
	{                                                                      \
		t temp = x;                                                    \
		x = y;                                                         \
		y = temp;                                                      \
	}

int main(void)
{
	char x = 'L';
	char y = 'R';
	printf("%c <-> %c\n", x, y);
	swap(char, x, y);
	printf("%c <-> %c\n", x, y);
	return 0;
}

/* Output

L <-> R
R <-> L

*/