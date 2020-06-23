#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getword.h"

struct wordinfo {
	char *word;
};

int getword(struct stream stream, char *word, int lim)
{
	if (getwordinfo(stream, word, lim) == NULL)
		return EOF;
	return *word;
}

struct wordinfo *getwordinfo(struct stream stream, char *word, int lim)
{
	int c;
	char *w = word;
	struct wordinfo *wi =
		(struct wordinfo *)malloc(sizeof(struct wordinfo));

	while (isspace(c = getch(stream)))
		;
	if (c == EOF)
		return NULL;
	*w++ = c;
	if (asalpha(c)) {
		for (; --lim > 0; w++)
			if (!asalnum(*w = getch(stream))) {
				ungetch(stream, *w);
				break;
			}
	}
	*w = '\0';
	wi->word = strdup(word);
	return wi;
}

/* is character we treat As alphabetic */
int asalpha(char c)
{
	return isalpha(c) || c == '_';
}

/* is character we treat As alphabetic or is numeric */
int asalnum(char c)
{
	return isalnum(c) || c == '_';
}
