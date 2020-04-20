/*
 * Exercise 5-1
 *
 * As written, getint treats a + or - not followed by a digit as a valid
 * representation of zero.  Fix it to push such a character back on the
 * input.
 */

#include <ctype.h>
#include <stdio.h>
#define SIZE 100
#define MAGIC -37
#define BUFSIZE 100

int getint(int *pn);
int getch(void);
void ungetch(int c);

char buf[BUFSIZE];
int bufp = 0;

int main(void)
{
	int n, array[SIZE], getint(int *);

	for (n = 0; n < SIZE; n++)
		array[n] = MAGIC;

	for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++)
		;

	printf("[ ");
	for (n = 0; n < SIZE && array[n] != MAGIC; n++)
		printf("%d ", array[n]);
	printf("]\n");

	return 0;
}

int getint(int *pn)
{
	int c, n, sign;

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
	for (*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');
	*pn *= sign;
	if (c != EOF)
		ungetch(c);
	return c;
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

ch5 [master|●2✚ 1]> echo "0 1 2 3 -45 +67 + -" | a.out
[ 0 1 2 3 -45 67 ]

*/