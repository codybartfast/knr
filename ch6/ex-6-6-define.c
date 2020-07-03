/*
 * Exercise 6-6
 *
 * Implement a simple version of the #define processor (i.e., no arguments)
 * suitable for use with C programs, based on the routines of this section.  You
 * may also find getch and ungetch helpful.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "filter-code.h"
#include "table.h"

#define MAXWORD 1000
#define LETTER char
#define DISPLAY printf
#define ENDWITH return

static struct charinfo *stored;
char *gettoken(void);
char *code(char c);
char *replacetkn(char *t);
char *preproc(char c);
int asalpha(char c);
int asalnum(char c);

char token[MAXWORD];
enum filtermode mode;

int main(void)
{
	LETTER *t;
	while ((t = gettoken()) != NULL)
		DISPLAY("%s", t);
	ENDWITH 0;
}

char *gettoken(void)
{
	struct charinfo *ci;
	char *t;

	if (stored != NULL) {
		ci = stored;
		stored = NULL;
	} else {
		ci = getparsed();
	}

	if (ci->ch == EOF) {
		freeci(ci);
		return NULL;
	}

	mode = ci->mode;
	switch (mode) {
	case CODE:
		t = code(ci->ch);
		if (t == NULL)
			return gettoken();
		else
			return t;
	case PREPROC:
		t = preproc(ci->ch);
		if (t == NULL)
			return gettoken();
		else
			return t;
	default:
		token[0] = ci->ch;
		token[1] = '\0';
		return token;
	}
}

char *code(char c)
{
	struct charinfo *ci;
	char *t = token;

	*t++ = c;
	if (asalpha(c)) {
		while ((ci = getparsed())->mode == CODE &&
		       asalnum(c = ci->ch)) {
			*t++ = c;
			freeci(ci);
		}
		stored = ci;
	}
	*t = '\0';
	return replacetkn(token);
}

char *replacetkn(char *name)
{
	char *defn = lookup(name);
	return (defn == NULL) ? name : defn;
}

char *preproc(char c)
{
	struct charinfo *ci;
	char *name, *endname, *defn, *p, *t = token;

	if (c != '#') {
		printf("error: Expected '#' at start of preproc, got %d\n", c);
		return NULL;
	}
	*(t++) = c;
	while ((ci = getparsed())->mode == PREPROC)
		*(t++) = ci->ch;
	stored = ci;
	*t = '\0';
	while (t > token && isspace(*(t - 1)))
		*--t = '\0';
	if (strncmp(token, "#define", 7) != 0 || !isspace(token[7]))
		return token;

	p = token + 7;
	while (isspace(*++p))
		;
	name = p;
	while (asalnum(*++p))
		;
	endname = p;
	while (isspace(*++p))
		;
	defn = p;
	*endname = '\0';
	if (p > t) {
		printf("error: Failed to parse #define: %s", token);
		return NULL;
	}
	install(name, defn);
	return NULL;
}

int asalpha(char c)
{
	return isalpha(c) || (c == '-');
}

int asalnum(char c)
{
	return isalnum(c) || asalpha(c);
}
