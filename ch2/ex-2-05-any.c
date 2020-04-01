/*
 * Exercise 2-5
 *
 * Write the function any(s1, s2), which returns the first location in the
 * string s1 wher any character from the string s2 occurs, or -1 if s1
 * contains no characters from s2.  (The standard library function strpbrk
 * does the same job but returns a pointer to the locations.)
 */

#include <stdio.h>

enum bool { NO, YES };

void test(char s1[], char s2[], int expected);
int any(char s1[], char s2[]);
int contains(char str[], char c);

int main(void)
{
	test("abcdefghij", "aeiou", 0);
	test("Abcdefghij", "aeiou", 4);
	test("", "aeiou", -1);
	test("Abcdefghij", "", -1);

	return 0;
}

void test(char s1[], char s2[], int expected)
{
	int actual = any(s1, s2);

	if (actual == expected) {
		printf("ok: \"%s\", \"%s\" -> \"%d\"\n", s1, s2, actual);
	} else {
		printf("FAIL: \"%s\", \"%s\" -> %d (expected \"%d\")\n", s1, s2,
		       actual, expected);
	}
}

int any(char s1[], char s2[])
{
	int i, j;

	for (i = j = 0; s1[i] != '\0'; i++)
		if (contains(s2, s1[i]))
			return i;
	return -1;
}

int contains(char str[], char c)
{
	int i = 0;

	while (str[i] != '\0')
		if (c == str[i++])
			return YES;
	return NO;
}
