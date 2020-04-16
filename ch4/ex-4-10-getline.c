/* Exercise 4-10
 *
 * An alternate organization uses getline to read an entire input line; this
 * makes getch and ungetch unnecessary.  Revise the calculator to use this
 * approach.
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

#define MAXLINE 1000

int apply(char[]);
double lookup(char[]);
double assign(double, int);
int getop(char[]);
void push(double);
double pop(void);
void peek(void);
void duplicate(void);
void swap(void);
void clear(void);
int get_line(char[], int);

int sp = 0;
double val[MAXVAL];

double vars[26];
double recent = 0.0;

char line[MAXLINE];
int linep;

int main(void)
{
	int i, type, doneln;
	double op2;
	char s[MAXOP];

	for (i = 0; i < 26; i++)
		vars[i] = 0.0;

	while (get_line(line, MAXLINE)) {
		doneln = linep = 0;
		while (!doneln) {
			type = getop(s);
			switch (type) {
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
			case '\n':
				printf("\t%.8g\n", recent = pop());
				doneln = 1;
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
			}
		}
	}
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

int getop(char s[])
{
	int i, c, next;

	i = 0;
	while ((s[0] = c = line[linep++]) == ' ' || c == '\t')
		;
	if (isalpha(c)) {
		while (isalnum(s[++i] = c = line[linep++]))
			;
		if (c == ADDRSUFX)
			s[i + 1] = '\0';
		else {
			s[i] = '\0';
			if (c != EOF)
				linep--;
		}
		return WORD;
	}
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-') {
		return (c == '\0') ? '\n' : c;
	}
	if (c == '-') {
		next = line[linep];
		if (!isdigit(next) && next != '.')
			return c;
	}
	if (isdigit(c) || c == '-')
		while (isdigit(s[++i] = c = line[linep++]))
			;
	if (c == '.')
		while (isdigit(s[++i] = c = line[linep++]))
			;
	s[i] = '\0';
	return NUMBER;
}

int get_line(char s[], int lim)
{
	int c, i;
	for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i)
		if (i < lim - 1)
			s[i] = c;
	if (c == '\n') {
		if (i < lim - 1)
			s[i] = c;
		++i;
	}
	if (i < lim)
		s[i] = '\0';
	else
		s[lim - 1] = '\0';
	return i;
}

/* Sample

2 3 +
    5

1 2 - 4 5 + * n' @ -1 $ * m' @ n m *
    45

*/