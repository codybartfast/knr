/*
 * Exercise 5-5.
 * 
 * Write versions of the library functions strncpy, strncat, and strncmp, 
 * which operate on at most the first n characters of their argument 
 * strings.  For example, strncpy(s, t, n) copies at most n characters of t
 * to s.  Full descriptions are in Appendix B.
 */

#include <stdio.h>

#define SIZE 1024

void test(int, char *, char *, int, char *);
void nstrcpy(char *, char *, int);
void nstrcat(char *, char *, int);
int nstrcmp(char *, char *, int);

enum fun { CPY, CAT, CMP };

int main(void)
{
	test(CPY, "Apple ", "Pears", 15, "Pears");
	test(CPY, "Apple ", "Pears", 4, "Pear");

	test(CAT, "Apple ", "Pears", 15, "Apple Pears");
	test(CAT, "Apple ", "Pears", 4, "Apple Pear");

	test(CMP, "Apple ", "Apple ", 15, "0");
	test(CMP, "Apple ", "Apple", 15, " ");
	test(CMP, "Apple", "Apple ", 15, " ");
	test(CMP, "Apple ", "Apple", 5, "0");
	return 0;
}

void test(int fun, char *s, char *t, int n, char *expected)
{
	int r;
	char writable[SIZE];

	nstrcpy(writable, s, SIZE);
	switch (fun) {
	case CPY:
		nstrcpy(writable, t, n);
		break;
	case CAT:
		nstrcat(writable, t, n);
		break;
	case CMP:
		r = nstrcmp(writable, t, n);
		writable[0] = r ? (r >= 0 ? r : -r) : '0';
		writable[1] = '\0';
		break;
	default:
		printf("error: Unknown function: %d\n", fun);
	}

	if (nstrcmp(writable, expected, SIZE) == 0) {
		printf("ok: \"%s\", \"%s\" -> \"%s\"\n", s, t, writable);
	} else {
		printf("FAIL: \"%s\", \"%s\" -> \"%s\" (expected \"%s\")\n", s,
		       t, writable, expected);
	}
}

void nstrcpy(char *s, char *t, int n)
{
	int cnt;

	for (cnt = 0; *t != '\0' && cnt++ < n; s++, t++)
		*s = *t;
	*s = '\0';
}

void nstrcat(char *s, char *t, int n)
{
	int cnt;

	while (*s)
		s++;
	for (cnt = 0; *t != '\0' && cnt++ < n; s++, t++)
		*s = *t;
	*s = '\0';
}

int nstrcmp(char *s, char *t, int n)
{
	int cnt;

	for (cnt = 0; *s == *t && cnt++ < n; s++, t++)
		if (*s == '\0' || n == cnt)
			return 0;
	return *s - *t;
}
