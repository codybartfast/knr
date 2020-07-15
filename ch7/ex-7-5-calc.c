/*
 * Exercise 7-5.
 *
 * Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf to do
 * the input and number cnversion.
 */

/* 
 * This solution is a cheat becaue I changed to spec to use '=' instead of '\n'
 * to terminate a calculation, and I use strtod instead of scan/sscanf to do the
 * number conversion.  Without these we seem to end up scanning for %c which
 * doesn't seem any different from just using getchar().
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'

int getop(double *);
void push(double);
double pop(void);

int getop(double *d)
{
	char *endptr;
	char s[MAXOP];

	if (scanf("%s", s) != 1)
		return EOF;
	*d = strtod(s, &endptr);
	/* check whole string was consumed */
	if (*endptr == '\0')
		return NUMBER;
	/* as not a number we should have length 1 */
	if (s[1] != '\0') {
		printf("error: don't know how to parse: %s\n", s);
		return EOF;
	}
	return s[0];
}

int main(void)
{
	int type;
	double op2;
	double d;

	while ((type = getop(&d)) != EOF) {
		switch (type) {
		case NUMBER:
			push(d);
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
		case '=':
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %c\n", type);
			break;
		}
	}
	return 0;
}

#define MAXHEIGHT 100

int sp = 0;
double val[MAXHEIGHT];

void push(double f)
{
	if (sp < MAXHEIGHT)
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
