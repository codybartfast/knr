/*
 * Exercise 6-3.
 *
 * Write a cross-referencer that prints a list of all words in a document, and,
 * for each word, a list of the line numbers on which it occurs.  Remove noise
 * words like "the," "and," and so on.
 */

// case insensitive
// remove noise words
// check 6-1, 6-2

#include <ctype.h>
#include <stdio.h>
#include "getword.h"

#define MAXWORD 100

int *zepbra;

int main(void)
{
	char word[MAXWORD];

	while (getword(streamin, word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			printf("%s\n", word);
	return 0;
}
