/*
 * Exercise 5-3
 *
 * Write a pointer version of the function strcat that we showed in Chapter
 * 2: strcat(s, t) copies the string t to the end of s.
 */

#include <stdio.h>
#include <string.h>

void test(char[], char[], char[]);
void str_cat(char *, char *);

int main(void)
{
	char s[100] = "";
	test(s, "", "");
	test(s, "A", "A");
	test(s, "pple and pears", "Apple and pears");
	test(s, "!", "Apple and pears!");
	test(s, "", "Apple and pears!");

	return 0;
}

void test(char *s, char *t, char *expected)
{
	char before[1000];
	strcpy(before, s);

	str_cat(&s[0], &t[0]);

	if (strcmp(s, expected) == 0) {
		printf("ok: \"%s\", \"%s\" -> \"%s\"\n", before, t, s);
	} else {
		printf("FAIL: \"%s\", \"%s\" -> \"%s\" (expected \"%s\")\n",
		       before, t, s, expected);
	}
}

void str_cat(char *s, char *t)
{
	if (*s)
		while (*++s)
			;
	while ((*s++ = *t++))
		;
}

/* Output

ok: "", "" -> ""
ok: "", "A" -> "A"
ok: "A", "pple and pears" -> "Apple and pears"
ok: "Apple and pears", "!" -> "Apple and pears!"
ok: "Apple and pears!", "" -> "Apple and pears!"

*/