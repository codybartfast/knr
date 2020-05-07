/* Exercise 5-14
 *
 * Modify the sort program to handle a -r flag, which indicates sorting in
 * reverse (decreasing) order.  Be sure that -r works with -n.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lines.h"
#include "quicksort.h"

int cmpstr(char *, char *);
int numcmp(char *, char *);
double atof(char s[]);

int reverse(void *a, void *b);

int (*basecompare)(void *, void *);
int (*compare)(void *, void *);

int main(int argc, char *argv[])
{
	int i, nlines;
	int numeric = 0;
	int doreverse = 0;

	char *buff, **lines;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-n") == 0)
			numeric = 1;
		else if (strcmp(argv[i], "-r") == 0)
			doreverse = 1;
	}

	basecompare = numeric ? ((int (*)(void *, void *))numcmp) :
				((int (*)(void *, void *))cmpstr);
	compare = doreverse ? reverse : basecompare;

	if ((nlines = readlines(&buff, &lines)) == LNS_ERROR) {
		printf("input too big to sort\n");
		return 0;
	} else {
		quicksort((void **)lines, 0, nlines - 1, compare);
		writelines(lines, nlines);
	}
	freelines(buff, lines);
	return 0;
}

int reverse(void *a, void *b)
{
	return (*basecompare)(b, a);
}

int cmpstr(char *s, char *t)
{
	int diff;
	for (; *s == *t; s++, t++)
		if (*s == '\0')
			return 0;

	diff = *s - *t;
	return diff == 0 ? 0 : diff > 0 ? 1 : -1;
}

int numcmp(char *s1, char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

double atof(char s[])
{
	double val, power;
	int i, sign, expsign, exp;

	for (i = 0; isspace(s[i]); i++)
		;
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if (s[i] == '.')
		i++;
	for (power = 1.0; isdigit(s[i]); i++) {
		val = 10.0 * val + (s[i] - '0');
		power *= 10.0;
	}
	if (s[i] == 'e' || s[i] == 'E') {
		i++;
		expsign = (s[i] == '-') ? -1 : 1;
		if (s[i] == '+' || s[i] == '-')
			i++;
		for (exp = 0; isdigit(s[i]); i++)
			exp = 10 * exp + (s[i] - '0');
		while (exp-- > 0)
			power = (expsign == 1) ? power / 10.0 : power * 10.0;
	}
	return sign * val / power;
}
