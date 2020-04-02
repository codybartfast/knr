/*
 * Exercise 2-10
 *
 * Rewrite the function lower, which converts upper case letters to lower
 * case, with a conditional expression instead of if-else.
 */

#include <stdio.h>

void test(int x, int expected);
int lower(int x);

int main(void)
{
	test('a', 'a');
	test('m', 'm');
	test('z', 'z');
	test('=', '=');
	test('A', 'a');
	test('M', 'm');
	test('Z', 'z');

	return 0;
}

void test(int x, int expected)
{
	int actual = lower(x);

	if (actual == expected) {
		printf("ok: x:%c -> %c\n", x, actual);
	} else {
		printf("FAIL: x:%c, -> %c (expected %c)\n", x, actual,
		       expected);
	}
}

int lower(int x)
{
	return (x < 'A' || 'Z' < x) ?  x : x + 'a' - 'A';
}
