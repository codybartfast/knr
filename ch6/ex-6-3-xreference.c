/*
 * Exercise 6-3.
 *
 * Write a cross-referencer that prints a list of all words in a document, and,
 * for each word, a list of the line numbers on which it occurs.  Remove noise
 * words like "the," "and," and so on.
 */

// malloc getword/strdup,wordinfo
// case insensitive
// remove noise words
// check 6-1, 6-2
// free

#include <ctype.h>
#include <stdio.h>
#include "getword.h"

#define MAXWORD 100

int *zepbra;

int main(void)
{
	struct wordinfo *wi;
	char word[MAXWORD];

	while ((wi = getwordinfo(&streamin, word, MAXWORD)) != NULL)
		printf("%d:%d %s\n", wi->line, wi->pos, wi->word);
	return 0;
}
