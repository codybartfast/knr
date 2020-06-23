#include <ctype.h>
#include <stdio.h>
#include "getword.h"

int getword(struct stream stream, char *word, int lim)
{
	return getwordinfo(stream, word, lim);
}

int getwordinfo(struct stream stream, char *word, int lim)
{
	int c;
	char *w = word;

	while (isspace(c = getch(stream)))
		;
	if (c != EOF)
		*w++ = c;
	if (!asalpha(c)) {
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++)
		if (!asalnum(*w = getch(stream))) {
			ungetch(stream, *w);
			break;
		}
	*w = '\0';
	return word[0];
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
