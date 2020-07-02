/*
 * Exercise 6-1
 *
 * Our version of getword does not properly handle underscores, string
 * constants, comments or preprocesor control lines.  Write a better version.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "filter-code.h"
#include "getword.h"

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key))

struct key {
	char *word;
	int count;
} keytab[] = { { "auto", 0 },	  { "break", 0 },    { "case", 0 },
	       { "char", 0 },	  { "const", 0 },    { "continue", 0 },
	       { "default", 0 },  { "do", 0 },	     { "double", 0 },
	       { "else", 0 },	  { "enum", 0 },     { "extern", 0 },
	       { "float", 0 },	  { "for", 0 },	     { "goto", 0 },
	       { "if", 0 },	  { "int", 0 },	     { "long", 0 },
	       { "register", 0 }, { "return", 0 },   { "short", 0 },
	       { "signed", 0 },	  { "sizeof", 0 },   { "static", 0 },
	       { "struct", 0 },	  { "switch", 0 },   { "typedef", 0 },
	       { "union", 0 },	  { "unsigned", 0 }, { "void", 0 },
	       { "volatile", 0 }, { "while", 0 } };

int nkeys;

int binsearch(char *, struct key *, int);

int main(void)
{
	int n;
	char word[MAXWORD];
	nkeys = (sizeof keytab / sizeof(struct key));

	while (getword(FILTER_CODE, word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			if ((n = binsearch(word, keytab, nkeys)) >= 0)
				keytab[n].count++;
	for (n = 0; n < nkeys; n++)
		if (keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);
	return 0;
}

int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}
