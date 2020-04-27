/*
 * Exercise 5-10.
 *
 * Write the program expr, which evaluates a reverse Polish expresion from
 * the command line, where each operator or operand is a separate argument.
 * For example,
 *
 * 	expr 2 3 4 + *
 *
 * evaluates 2 x (3+4).
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#define MAXOP 100
#define NUMBER '0'
#define WORD '1'
#define ADDRSUFX '\''
#define NOTFOUND -1

#define MAXVAL 100

#define BUFSIZE 100

int apply(char[]);
double lookup(char[]);
double assign(double, int);
int gettype(char[]);
void push(double);
double pop(void);
void peek(void);
void duplicate(void);
void swap(void);
void clear(void);

int sp = 0;
double val[MAXVAL];

double vars[26];
double recent = 0.0;

int getch(void);
void ungetch(int);

char buf[BUFSIZE];
int bufp = 0;

int main(int argc, char *argv[])
{
	int i, type;
	double op2;
	char *s;

	for (i = 0; i < 26; i++)
		vars[i] = 0.0;

	for (i = 0; i < argc; i++) {
		switch (type = gettype(s = argv[i])) {
		case NUMBER:
			push(atof(s));
			break;
		case WORD:
			if (apply(s))
				;
			else
				push(lookup(s));
			break;
		case '@':
			op2 = pop();
			push(assign(pop(), op2));
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
		case '?':
			peek();
			break;
		case '"':
			duplicate();
			break;
		case '~':
			swap();
			break;
		case '`':
			clear();
			break;
		case '$':
			push(recent);
			break;

		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	printf("\t%.8g\n", recent = pop());
	return 0;
}

int apply(char word[])
{
	double op2;

	if (strcmp(word, "sin") == 0) {
		push(sin(pop()));
	} else if (strcmp(word, "cos") == 0) {
		push(cos(pop()));
	} else if (strcmp(word, "tan") == 0) {
		push(tan(pop()));
	} else if (strcmp(word, "asin") == 0) {
		push(asin(pop()));
	} else if (strcmp(word, "acos") == 0) {
		push(acos(pop()));
	} else if (strcmp(word, "atan") == 0) {
		push(atan(pop()));
	} else if (strcmp(word, "atan2") == 0) {
		op2 = pop();
		push(atan2(pop(), op2));
	} else if (strcmp(word, "exp") == 0) {
		push(exp(pop()));
	} else if (strcmp(word, "log") == 0) {
		push(log(pop()));
	} else if (strcmp(word, "pow") == 0) {
		op2 = pop();
		push(pow(pop(), op2));
	} else if (strcmp(word, "sqrt") == 0) {
		push(sqrt(pop()));
	} else if (strcmp(word, "ceil") == 0) {
		push(ceil(pop()));
	} else if (strcmp(word, "floor") == 0) {
		push(floor(pop()));
	} else {
		return 0;
	}
	return 1;
}

double lookup(char word[])
{
	char c;
	if (islower(c = word[0])) {
		if (word[1] == ADDRSUFX)
			return c - 'a';
		else
			return vars[c - 'a'];
	}
	printf("error: undefined %s\n", word);
	return 0.0;
}

double assign(double val, int addr)
{
	if (0 <= addr && addr <= 26)
		return vars[addr] = val;
	printf("error: invalid address %d\n", addr);
	return 0.0;
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

void peek(void)
{
	if (sp > 0)
		printf("peek: %g\n", val[sp - 1]);
	else
		printf("error: stack empty\n");
}

void duplicate(void)
{
	if (sp <= 0)
		printf("error: stack empty\n");
	else if (sp >= MAXVAL)
		printf("error: stack full, can't duplicate\n");
	else
		push(val[sp - 1]);
}

void swap(void)
{
	int t;
	if (sp <= 1) {
		printf("error: stack has fewer than two items\n");
	} else {
		t = val[sp - 2];
		val[sp - 2] = val[sp - 1];
		val[sp - 1] = t;
	}
}

void clear(void)
{
	sp = 0;
}

int gettype(char s[])
{
	int c, next;

	c = s[0];
	if (isalpha(c)) {
		return WORD;
	}
	if (!isdigit(c) && c != '.' && c != '-')
		return c;
	if (c == '-') {
		if (!isdigit(next = s[1]) && next != '.')
			return c;
	}
	return NUMBER;
}

/* Output

ch5> a.out 2 3 4 + "*" "g'" "@" 0 3 - "h'" @ g h +
    11

*/