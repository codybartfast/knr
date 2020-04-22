/*
 * Exercise 5-5.
 *
 * Rewrite appropriate programs from earlier chapters and exercises with
 * pointers instead of array indexing.  Good possibilities include getline
 * (Chapters 1 and 4), atoi, itoa, and their variants (Chapters 2, 3, and
 * 4), reverse (Chapter 3), and strindex and getop (Chapter 4).
 */

#include <stdio.h>
#include <string.h>

void test(char *s, char *expected);
void reverse(char *);

int main(void)
{
	test("", "");
	test("A", "A");
	test("-|", "|-");
	test("Apple ", " elppA");

	return 0;
}

void test(char *s, char *expected)
{
	char w[100];

	strcpy(w, s);
	reverse(w);

	if (strcmp(w, expected) == 0) {
		printf("ok: \"%s\",  -> \"%s\"\n", s, w);
	} else {
		printf("FAIL: \"%s\", -> \"%s\" (expected \"%s\")\n", s, w,
		       expected);
	}
}

void reverse(char *s) {
	char t, *e = s;

	while(*e++)
		;
	e -= 2;
	for(; s < e; s++, e--){
		t = *s;
		*s = *e;
		*e = t;
	}
}
