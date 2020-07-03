#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter-code.h"
#include "getword.h"

struct wordinfo *getwi(struct stream *stream, char *word, int lim);

char *pseudoalphas = "_";
int ispseudoalpha(char c);

int getword(int filter, char *word, int lim)
{
	struct wordinfo *wi = getwordinfo(filter, lim);

	if (wi == NULL)
		return EOF;
	strcpy(word, wi->word);
	free(wi);
	return *word;
}

struct wordinfo *getwordinfo(int filter, int lim)
{
	struct stream *stream = filter ? &filteredin : &streamin;
	char *word = (char *)malloc(lim * sizeof(char));
	if (word == NULL)
		return NULL;
	return getwi(stream, word, lim);
}

struct wordinfo *getwi(struct stream *stream, char *word, int lim)
{
	int c;
	char *w = word;
	struct wordinfo *wi;

	while (isspace(c = getch(stream)))
		;
	if (c == EOF) {
		free(word);
		return NULL;
	}
	*w++ = c;
	if (asalpha(c)) {
		wi = (struct wordinfo *)malloc(sizeof(struct wordinfo));
		if (wi == NULL) {
			free(word);
			return NULL;
		}
		wi->word = word;
		wi->line = stream->line + 1;
		wi->pos = stream->pos;
		for (; --lim > 0; w++) {
			if (!asalnum(*w = getch(stream))) {
				ungetch(stream, *w);
				break;
			}
		}
		*w = '\0';
		return wi;
	}
	return getwi(stream, word, lim);
}

/* is character we treat As alphabetic */
int asalpha(char c)
{
	return isalpha(c) || ispseudoalpha(c);
}

/* is character we treat As alphabetic or is numeric */
int asalnum(char c)
{
	return isalnum(c) || asalpha(c);
}

int ispseudoalpha(char c)
{
	char *pa;
	for (pa = pseudoalphas; *pa != '\0'; pa++)
		if (*pa == c)
			return 1;
	return 0;
}

void freewordinfo(struct wordinfo *wi)
{
	if (wi == NULL)
		return;
	free(wi->word);
	free(wi);
}
