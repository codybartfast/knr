/*
 * Exercise 3-5
 *
 * Write the function itob(n, s, b) that converts the integer n into a base
 * b character representation in the string s.  In paricular, itob(n, s, 16)
 * formats n as a hexadecimal integer in s.
 */

#include <stdio.h>
#include <string.h>

void test(int n, int b, char actual[]);
void itob(int n, int b, char s[]);
void reverse(char s[]);

int main(void)
{
	test(123, 2, "1111011");
	test(123, 8, "173");
	test(123, 10, "123");
	test(123, 16, "7b");
	test(123, 32, "3r");
	test(2147483647, 16, "7fffffff");
	test(-2147483648, 16, "-80000000");
	return 0;
}

void test(int n, int b, char expected[])
{
	char actual[100];

	itob(n, b, actual);

	if (strcmp(actual, expected) == 0) {
		printf("ok: %d base %d -> \"%s\"\n", n, b, actual);
	} else {
		printf("FAIL: %d base %d -> \"%s\" (expected \"%s\")\n", n, b,
		       actual, expected);
	}
}

void itob(int n, int b, char s[])
{
	int i, sign, digit;
	int alpha = 'a' - '9' - 1;

	/*
	 * If we're getting floor quotient instead of 'normal' truncated
	 * quotient, then % will give positive remainders for negative
	 * dividends and we need to subract b to get 'normal' negative
	 * remainders.
	 */

	int adj = ((-13 / 10) == -2) ? b : 0;

	if ((sign = n) > 0)
		n = -n;
	i = 0;
	do {
		digit = -((n % b) - adj);
		digit = digit > 9 ? digit + alpha : digit;
		s[i++] = '0' + digit;
	} while ((n /= b) < 0);
	if (sign < 0)
		s[i++] = '-';
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

ok: 123 base 2 -> "1111011"
ok: 123 base 8 -> "173"
ok: 123 base 10 -> "123"
ok: 123 base 16 -> "7b"
ok: 123 base 32 -> "3r"
ok: 2147483647 base 16 -> "7fffffff"
ok: -2147483648 base 16 -> "-80000000"

*/
