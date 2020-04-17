/*
 * Exercise 4-13
 *
 * Write a recursive version of the function reverse(s), which reverses the
 * string s in place.
 */

#include <stdio.h>
#include <string.h>

void test(char[], char[]);
void rev(char s[]);
void reverse(char[]);
void reverser(char[], int, int);

int main(void)
{
	char s1[100] = "abcdefg";
	char s2[100] = "abcdefgh";

	test("", "");
	test("a", "a");
	test(s1, "gfedcba");
	test(s2, "hgfedcba");

	return 0;
}

void test(char s[100], char expected[])
{
	char before[100];
	strcpy(before, s);
	reverse(s);

	if (strcmp(s, expected) == 0) {
		printf("ok: %s -> \"%s\"\n", before, s);
	} else {
		printf("FAIL: %s -> \"%s\" (expected \"%s\")\n", before, s,
		       expected);
	}
}

void reverse(char s[100])
{
	reverser(s, 0, strlen(s) - 1);
}

void reverser(char s[100], int left, int right)
{
	char t;

	if (left < right) {
		t = s[left];
		s[left] = s[right];
		s[right] = t;
		reverser(s, left + 1, right - 1);
	}
}
