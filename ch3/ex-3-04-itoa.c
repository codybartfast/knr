/*
 * Exercise 3-4
 *
 * In a two's complement number representation, our version of itoa does not
 * handle the largest negative number, that is, the value of n equal to
 * -(2^(wordsize-1)).  Explain why not,  Modify it to print that value
 * correctly, regardless of the machine on which it runs.
 */

#include <stdio.h>
#include <string.h>

void test(int n, char actual[]);
void itoa(int n, char s[]);
void itoa89(int n, char s[]);
void reverse(char s[]);

char explanation[] =
	"Itoa does not work with the minimum value because for negative numbers it\n"
	"relies on negating the value.  But the magnitude of the minimum value is one\n"
	"greater than the magnitude of the maximum value.  i.e., (2^(wordsize-1)) is\n"
	"not representable.  Infact the bit arrangement that would represent\n"
	"(2^(wordsize-1)) in an unsigned (or longer) int, is -(2^(wordsize-1))\n"
	"for an unsigned int\n\n";

int main(void)
{
	printf("%s", explanation);
	test(1, "1");
	test(-1, "-1");
	test(123, "123");
	test(-123, "-123");
	test(2147483647, "2147483647");
	test(-2147483648, "-2147483648");

	return 0;
}

void test(int n, char expected[])
{
	char actual[100];

	itoa89(n, actual);

	if (strcmp(actual, expected) == 0) {
		printf("ok: %d -> \"%s\"\n", n, actual);
	} else {
		printf("FAIL: %d -> \"%s\" (expected \"%s\")\n", n, actual,
		       expected);
	}
}

void itoa(int n, char s[])
{
	int i, sign;
	if ((sign = n) > 0)
		n = -n;
	i = 0;
	do {
		s[i++] = '0' - (n % 10);
	} while ((n /= 10) < 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void itoa89(int n, char s[])
{
	int i, sign;

	/*
	 * Test if we're getting floor quotient -2, instead of 'normal'
	 * truncated quotient, -1.  If so, % will give positive remainders
	 * for negative dividends and we need to subract 10 to get 'normal'
	 * negative remainders.
	 */

	int adj = ((-13 / 10) == -2) ? 10 : 0;

	if ((sign = n) > 0)
		n = -n;
	i = 0;
	do {
		/* adj will ensure we get a negative remainder */
		s[i++] = '0' - ((n % 10) - adj);
	} while ((n /= 10) < 0);
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
