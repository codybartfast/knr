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

static struct charinfo *stored;
char *gettoken(void);
char *code(char c);
char *checkdefines(char *t);
char *preproc(char c);
int asalpha(char c);
int asalnum(char c);

char token[MAXWORD];
char *ERROR = "ERROR";

int main(void)
{
	char *t;
	while ((t = gettoken()) != NULL && t != ERROR)
		printf("%s", t);
	return 0;
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

	switch (ci->mode) {
	case CODE:
		t = code(ci->ch);
		break;
	case PREPROC:
		t = preproc(ci->ch);
		break;
	default:
		token[0] = ci->ch;
		token[1] = '\0';
		break;
	}
	freeci(ci);
	return (t == NULL) ? gettoken() : t;
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
	return checkdefines(token);
}

char *checkdefines(char *name)
{
	char *defn = lookup(name);
	return (defn == NULL) ? name : defn;
}

char *preproc(char c)
{
	struct charinfo *ci;
	char *name, *d, *t = token;

	if (c != '#') {
		printf("error: Expected '#' at start of preproc, got %d\n", c);
		return ERROR;
	}
	*(t++) = c;

	/* copy preprocessor statement to token */
	while ((ci = getparsed())->mode == PREPROC) {
		*(t++) = ci->ch;
		freeci(ci);
	}
	stored = ci;
	*t = '\0';

	/* if not #define return as is */
	if (strncmp(token, "#define", 7) != 0 || !isspace(token[7]))
		return token;

	/* trim trailing space */
	while (t > token && isspace(*(t - 1)))
		*--t = '\0';

	/* trim whitespace before name */
	d = token + 7;
	while (isspace(*++d))
		;

	/* set name */
	name = d;
	if (!asalpha(*name)) {
		printf("error: Expected #define identifier (%s).\n", token);
		return ERROR;
	}
	while (asalnum(*++d))
		;

	/* advance to defin */
	if (*d != '\0') {
		if (!isspace(*d)) {
			/* gcc doesn't require this */
			printf("error: Expected space after identifier: (%s)\n",
			       token);
			return ERROR;
		}
		*d = '\0';
		while (isspace(*++d))
			;
	}
	install(name, d);
	return NULL;
}

int asalpha(char c)
{
	return isalpha(c) || (c == '_');
}

#define SORT_OF_LETTER asalpha(c)
#define NUMERAL isdigit(c)

int asalnum(char c)
{
	return SORT_OF_LETTER || NUMERAL;
}
