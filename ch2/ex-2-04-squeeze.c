/*
 * Exercise 2-4
 *
 * Write an alternate version squeeze (s1, s2) that deletes each character 
 * in s1 that matches any character in the string s2.
 */

#include <stdio.h>
#include <string.h>

enum bool { NO, YES };

void test(char s1[], char s2[], char expected[]);
void squeeze(char s1[], char s2[]);
int contains(char str[], char c);

int main(void)
{
	char str[] = "abcdefghij";
	test(str, "aeiou", "bcdfghj");

	return 0;
}

void test(char s1[], char s2[], char expected[])
{
	char before[1000];

	strcpy(before, s1);
	squeeze(s1, s2);
	if (strcmp(s1, expected) == 0) {
		printf("ok: \"%s\" -> \"%s\"\n", before, s1);
	} else {
		printf("FAIL: \"%s\" -> %s (expected \"%s\")\n", before, s1,
		       expected);
	}
}

void squeeze(char s1[], char s2[])
{
	int i, j;

	for (i = j = 0; s1[i] != '\0'; i++)
		if (!contains(s2, s1[i]))
			s1[j++] = s1[i];
	s1[j] = '\0';
}

int contains(char str[], char c)
{
	int i = 0;

	while (str[i] != '\0')
		if (c == str[i++])
			return YES;
	return NO;
}
