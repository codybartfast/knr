#include <ctype.h>
#include <stdio.h>
#include "filter-code.h"

int getword(char *, int);
int asalpha(char c);
int asalnum(char c);

int filtered(void);
struct filterstate filterstate;
int filterbuff[MAXCHBUF];
struct stream filteredstream = { filtered, filterbuff, 0 };

int filtered(void)
{
	return filter_code(streamin, filterstate);
}

int getword(char *word, int lim)
{
	int c;
	char *w = word;

	while (isspace(c = getch(filteredstream)))
		;
	if (c != EOF)
		*w++ = c;
	if (!asalpha(c)) {
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++)
		if (!asalnum(*w = getch(filteredstream))) {
			ungetch(filteredstream, *w);
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
