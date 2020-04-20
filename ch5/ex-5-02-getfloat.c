/*
 * Exercise 5-2
 *
 * Write getfloat, the floating-point analog of getint.  What type does
 * getfloat return as its fucntion value?
 */

#include <ctype.h>
#include <stdio.h>
#define SIZE 100
#define MAGIC -37
#define BUFSIZE 100

int getfloat(float *);
int getch(void);
void ungetch(int c);

char buf[BUFSIZE];
int bufp = 0;

int main(void)
{
	int n;
	float array[SIZE];

	for (n = 0; n < SIZE; n++)
		array[n] = MAGIC;

	for (n = 0; n < SIZE && getfloat(&array[n]) != EOF; n++)
		;

	printf("[ ");
	for (n = 0; n < SIZE && array[n] != MAGIC; n++)
		printf("%g ", array[n]);
	printf("]\n");

	return 0;
}

int getfloat(float *pn)
{
	double val, power;
	int c, n, sign, expsign, exp;

	while (isspace(c = getch()))
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {
		if (!isdigit(n = getch())) {
			ungetch(n);
			ungetch(c);
			return 0;
		} else {
			c = n;
		}
	}
	for (val = 0.0; isdigit(c); c = getchar())
		val = 10.0 * val + (c - '0');
	if (c == '.')
		c = getchar();
	for (power = 1.0; isdigit(c); c = getchar()) {
		val = 10.0 * val + (c - '0');
		power *= 10.0;
	}
	if (c == 'e' || c == 'E') {
		c = getchar();
		expsign = (c == '-') ? -1 : 1;
		if (c == '+' || c == '-')
			c = getchar();
		for (exp = 0; isdigit(c); c = getchar())
			exp = 10 * exp + (c - '0');
		while (exp-- > 0)
			power = (expsign == 1) ? power / 10.0 : power * 10.0;
	}
	*pn = sign * val / power;
	return 0;
}

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

/* Output

ch5 [master ↑·1|…1]> echo "45 45.321 -45.321 45.321e+0 45.321e+1 45.321e+2
	45.321e-1 45.321e-2 + - 42" | a.out
[ 45 45.321 -45.321 45.321 453.21 4532.1 4.5321 0.45321 ]

*/