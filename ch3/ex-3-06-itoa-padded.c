/*
 * Exercise 3-6
 *
 * Write a version of itoa that accepts three arguments instead of two.  The
 * third argument is a minimum field width; the converted number must be
 * padded with blanks on the left if necessary to make it wide enought.
 */

#include <stdio.h>
#include <string.h>

void test(int n, int w, char actual[]);
void itoa(int n, int w, char s[]);
void reverse(char s[]);

int main(void)
{
	test(1, 4, "   1");
	test(-1, 4, "  -1");
	test(123, 4, " 123");
	test(-123, 4, "-123");
	test(2147483647, 4, "2147483647");
	test(-2147483648, 4, "-2147483648");

	return 0;
}

void test(int n, int w, char expected[])
{
	char actual[100];

	itoa(n, w, actual);

	if (strcmp(actual, expected) == 0) {
		printf("ok: %d, %d -> \"%s\"\n", n, w, actual);
	} else {
		printf("FAIL: %d, %d -> \"%s\" (expected \"%s\")\n", n, w,
		       actual, expected);
	}
}

void itoa(int n, int w, char s[])
{
	int i, sign;
	int adj = ((-13 / 10) == -2) ? 10 : 0;

	if ((sign = n) > 0)
		n = -n;
	i = 0;
	do {
		s[i++] = '0' - ((n % 10) - adj);
	} while ((n /= 10) < 0);
	if (sign < 0)
		s[i++] = '-';
	while (i < w)
		s[i++] = ' ';
	s[i] = '\0';
	reverse(s);
}

void reverse(char s[])
{
	int c, i, j;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* Output

ok: 1, 4 -> "   1"
ok: -1, 4 -> "  -1"
ok: 123, 4 -> " 123"
ok: -123, 4 -> "-123"
ok: 2147483647, 4 -> "2147483647"
ok: -2147483648, 4 -> "-2147483648"

*/
