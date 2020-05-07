/* Exercise 5-14
 *
 * Modify the sort program to handle a -r flag, which indicates sorting in
 * reverse (decreasing) order.  Be sure that -r works with -n.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lines.h"
#include "qsort.h"

void qsort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
void swap(void *v[], int, int);
int cmpstr(char *, char *);
int numcmp(char *, char *);
double atof(char s[]);

int main(int argc, char *argv[])
{
	int nlines;
	int numeric = 0;

	char *buff;
	char **lines;

	if (argc > 1 && cmpstr(argv[1], "-n") == 0)
		numeric = 1;

	if ((nlines = readlines(&buff, &lines)) == LNS_ERROR) {
		printf("input too big to sort\n");
		return 0;
	} else {
		qsort((void **)lines, 0, nlines - 1,
		      (int (*)(void *, void *))(numeric ? numcmp : cmpstr));
		writelines(lines, nlines);
	}
	freelines(buff, lines);
	return 0;
}


int cmpstr(char *s, char *t)
{
	for (; *s == *t; s++, t++)
		if (*s == '\0')
			return 0;
	return *s - *t;
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
