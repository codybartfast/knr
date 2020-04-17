/*
 * Exercise 4-12
 *
 * Adapt the ideas of printd to write a recursive version of itoa; that is,
 * convert an integer into a string by calling a recursive routine.
 */

#include <stdio.h>
#include <string.h>

void test(int n, char actual[]);
void itoa(int n, char s[]);
int itoar(int n, char s[], int i);

int adj = ((-3 / 10) == -1) ? 10 : 0;

int main(void)
{
	test(0, "0");
	test(1, "1");
	test(-1, "-1");
	test(12, "12");
	test(-12, "-12");
	test(123, "123");
	test(-1000, "-1000");
	test(2147483647, "2147483647");
	test(-2147483648, "-2147483648");

	return 0;
}

void test(int n, char expected[])
{
	char actual[100];

	itoa(n, actual);

	if (strcmp(actual, expected) == 0) {
		printf("ok: %d -> \"%s\"\n", n, actual);
	} else {
		printf("FAIL: %d -> \"%s\" (expected \"%s\")\n", n, actual,
		       expected);
	}
}

void itoa(int n, char s[])
{
	int i = 0;

	if (n < 0)
		s[i++] = '-';
	else
		n = -n;
	s[itoar(n, s, i)] = '\0';
}

int itoar(int n, char s[], int i)
{
	if (n <= -10)
		i = itoar(n / 10, s, i);
	s[i++] = '0' - ((n % 10) - adj);
	return i;
}

/* Output

ok: 0 -> "0"
ok: 1 -> "1"
ok: -1 -> "-1"
ok: 12 -> "12"
ok: -12 -> "-12"
ok: 123 -> "123"
ok: -100 -> "-100"
ok: 2147483647 -> "2147483647"
ok: -2147483648 -> "-2147483648"

*/