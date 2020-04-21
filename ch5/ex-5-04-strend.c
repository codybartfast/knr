/*
 * Exercise 5-3
 *
 * Write the function strend(s,t) , which returns 1 if the string t occurs
 * at the end of the string s, and zero otherwise.
 */

#include <stdio.h>

void test(char[], char[], int);
int strend(char *, char *);

int main(void)
{
	test("", "", 1);
	test("a", "a", 1);
	test("a", "", 1);
	test("", "a", 0);
	test("duck", "duck", 1);
	test("duck", "ducky", 0);
	test("Unsubtle", "subtle", 1);
	test("frankly", "frank", 0);
	test("aaaaaX", "aaaaX", 1);

	return 0;
}

void test(char *s, char *t, int expected)
{
	int actual = strend(s, t);

	if (actual == expected) {
		printf("ok: \"%s\", \"%s\" -> %d\n", s, t, actual);
	} else {
		printf("FAIL: \"%s\", \"%s\" -> %d (expected %d)\n", s, t,
		       actual, expected);
	}
}

int strend(char *s, char *t)
{
	char *cs, *ct;
	do {
		if (*s == *t) {
			cs = s, ct = t;
			do {
				if (*cs == '\0')
					return 1;
			} while (*++cs == *++ct);
		}
	} while (*s++ != '\0');
	return 0;
}

/* Output

	ok: "", "" -> 1
	ok: "a", "a" -> 1
	ok: "a", "" -> 1
	ok: "", "a" -> 0
	ok: "duck", "duck" -> 1
	ok: "duck", "ducky" -> 0
	ok: "Unsubtle", "subtle" -> 1
	ok: "frankly", "frank" -> 0
	ok: "aaaaaX", "aaaaX" -> 1

*/
