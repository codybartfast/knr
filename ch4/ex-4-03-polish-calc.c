/* Exercise 4-3
 *
 * Given the basic framework, it's straightforward to extend the calulator.
 * Add the modulus (%) operator and provisions for negative numbers
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>

#define MAXOP 100
#define NUMBER '0'

#define MAXVAL 100

#define BUFSIZE 100

int getop(char[]);
void push(double);
double pop(void);

int sp = 0;
double val[MAXVAL];

int getch(void);
void ungetch(int);

char buf[BUFSIZE];
int bufp = 0;

int main(void)
{
	int type;
	double op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '%':
			op2 = pop();
			if (op2 != 0.0)
				push((long)pop() % (long)op2);
			else
				printf("error: zero divisor (mod)\n");
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}

void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

int getop(char s[])
{
	int i, c, next;

	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;
	if (c == '-') {
		ungetch(next = getch());
		if (!isdigit(next) && next != '.')
			return c;
	}
	i = 0;
	if (isdigit(c) || c == '-')
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
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

/* Sample

1 2 - 4 5 + * -3 /
    3
-100
    -100
7 -3 -
    10

*/
