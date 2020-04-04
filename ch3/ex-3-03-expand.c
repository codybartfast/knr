/*
 * Exercise 3-3
 *
 * Write a function expand(s1, s2) that expands shorthand notations like a-z
 * in the string s1 into the equivalent complete list abc...xyz in s2.
 * Allow for letters of either case and digits, and be prepared to handle
 * cases like a-b-c and a-z0-9 and -a-z.  Arrange that a leading or trailing
 *  - is taken literally.
 */

/*
 * Deliberately not checking that the range is homogenous (e.g. digit -> 
 * digit or UPPER -> UPPER) because trying to emulate the behaviour of
 * regular expression ranges.
 */

#include <stdio.h>
#include <string.h>

#include <ctype.h>

enum bool { NO, YES };

void test(char str[], char actual[]);
void expand(char s1[], char s2[]);
int validrange(char s, char e);

int main(void)
{
	test("a-z", "abcdefghijklmnopqrstuvwxyz");
	test("A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	test("0-9", "0123456789");
	test("a-fG-M3-8", "abcdefGHIJKLM345678");
	test("a-b-c", "abc");
	test("a-c-e", "abcde");
	test("-a-c-", "-abc-");
	test("a-a", "aa");
	test("z-a", "za");
	test("W-d", "WXYZ[\\]^_`abcd");
	test("7-D", "789:;<=>?@ABCD");
	return 0;
}

void test(char str[], char expected[])
{
	char actual[1000];

	expand(str, actual);

	if (strcmp(actual, expected) == 0) {
		printf("ok: \"%s\" -> \"%s\"\n", str, actual);
	} else {
		printf("FAIL: \"%s\" -> \"%s\" (expected \"%s\")\n", str,
		       actual, expected);
	}
}

void expand(char s1[], char s2[])
{
	int i, j;
	char c, r;
	int expand_mode = NO;
	char prev = '\0';

	for (i = j = 0; (c = s1[i]); i++) {
		if (c == '-' && prev != '\0') {
			expand_mode = YES;
		} else {
			if (expand_mode)
				for (r = prev + 1; r < c; r++)
					s2[j++] = r;
			s2[j++] = prev = c;
			expand_mode = NO;
		}
	}
	if (expand_mode)
		s2[j++] = '-';
	s2[j] = '\0';
}

/*
 * Output
 * ======
 *	ok: "a-z" -> "abcdefghijklmnopqrstuvwxyz"
 *	ok: "A-Z" -> "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
 *	ok: "0-9" -> "0123456789"
 *	ok: "a-fG-M3-8" -> "abcdefGHIJKLM345678"
 *	ok: "a-b-c" -> "abc"
 *	ok: "a-c-e" -> "abcde"
 *	ok: "-a-c-" -> "-abc-"
 *	ok: "a-a" -> "aa"
 *	ok: "z-a" -> "za"
 *	ok: "W-d" -> "WXYZ[\]^_`abcd"
 *	ok: "7-D" -> "789:;<=>?@ABCD"
 */
