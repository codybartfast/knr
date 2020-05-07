/* Exercise 5-16
 *
 * Add the -d ("directory order") option, which makes comparisons only on 
 * letters, numers and blanks.  Make sure it works in conjunction with -f.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lines.h"
#include "quicksort.h"

int numcmp(char *, char *);

void freestuff(char ***kvms, int nlines);
void dirvalue(char *value);
void fold(char *value);
char *makekey(char *value, int dofold, int dodirsort);
char **keyvaluepair(char *key, char *value);
char *key(char *pair[]);
char *value(char *pair[]);
int keycompare(void *a, void *b);
int reverse(void *a, void *b);

int (*basecompare)(void *, void *);
int (*compare)(void *, void *);

int main(int argc, char *argv[])
{
	int i, nlines;
	int numeric = 0;
	int doreverse = 0;
	int dofold = 0;
	int dodirsort = 0;

	char *buff, **lines, ***kvps;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-n") == 0)
			numeric = 1;
		else if (strcmp(argv[i], "-r") == 0)
			doreverse = 1;
		else if (strcmp(argv[i], "-f") == 0)
			dofold = 1;
		else if (strcmp(argv[i], "-d") == 0)
			dodirsort = 1;
	}

	basecompare = numeric ? ((int (*)(void *, void *))numcmp) :
				((int (*)(void *, void *))strcmp);
	compare = doreverse ? reverse : basecompare;

	if ((nlines = readlines(&buff, &lines)) == LNS_ERROR) {
		printf("input too big to sort\n");
		return 0;
	}
	kvps = malloc(nlines * sizeof(char **));
	for (i = 0; i < nlines; i++) {
		char *key = makekey(lines[i], dofold, dodirsort);
		kvps[i] = keyvaluepair(key, lines[i]);
	}

	quicksort((void **)kvps, 0, nlines - 1, keycompare);

	for (i = 0; i < nlines; i++)
		lines[i] = value(kvps[i]);

	writelines(lines, nlines);

	freelines(buff, lines);
	freestuff(kvps, nlines);
	return 0;
}

void fold(char *value)
{
	while ((*value = toupper(*value)))
		value++;
}

void dirvalue(char *value)
{
	char c, *write;
	write = value;
	while ((c = *value++)) {
		if (isalnum(c) || c == ' ' || c == '\n')
			*write++ = c;
	}
	*write = '\0';
}

char *makekey(char *value, int dofold, int dodirsort)
{
	char *key = malloc((strlen(value) + 1) * sizeof(char));
	strcpy(key, value);
	if (dofold)
		fold(key);
	if (dodirsort)
		dirvalue(key);
	return key;
}

char **keyvaluepair(char key[], char value[])
{
	char **pair = malloc(2 * sizeof(char *));
	pair[0] = key;
	pair[1] = value;
	return pair;
}

char *key(char *pair[])
{
	return pair[0];
}

char *value(char *pair[])
{
	return pair[1];
}

int keycompare(void *a, void *b)
{
	char *v1 = key(a);
	char *v2 = key(b);
	return (*compare)(v1, v2);
}

int reverse(void *a, void *b)
{
	return (*basecompare)(b, a);
}

void freestuff(char ***kvps, int nlines)
{
	int i;
	for (i = 0; i < nlines; i++) {
		free(key(kvps[i]));
		free(kvps[i]);
	}
	free(kvps);
}

int numcmp(char *s1, char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}
