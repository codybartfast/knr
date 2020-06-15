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

int filtered(void);
int getword(char *, int);
int binsearch(char *, struct key *, int);
int asalpha(char c);
int asalnum(char c);

int nkeys;
struct filterstate filterstate;
int filterbuff[MAXCHBUF];
struct stream filteredstream = { filtered, filterbuff, 0 };

int main(void)
{
	int n;
	char word[MAXWORD];
	nkeys = (sizeof keytab / sizeof(struct key));

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			if ((n = binsearch(word, keytab, nkeys)) >= 0)
				keytab[n].count++;
	for (n = 0; n < nkeys; n++)
		if (keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);
	return 0;
}

/* like getch, but preproc, comments and strings are removed */
int filtered(void)
{
	return filter_code(streamin, filterstate);
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
int asalpha(char c){
	return isalpha(c) || c == '_';
}

/* is character we treat As alphabetic or is numeric */
int asalnum(char c){
	return isalnum(c) || c == '_';
}
