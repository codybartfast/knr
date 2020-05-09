/* Exercise 5-15
 *
 * Add the -d ("directory order") option, which makes comparisons only on 
 * letters, numers and blanks.  Make sure it works in conjunction with -f.
 */

#define UNSPEC -1

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lines.h"
#include "quicksort.h"

void parseargs(int argc, char *argv[]);

int numcmp(char *, char *);
int numcmpr(char *, char *);
int strcmpr(char *, char *);
void freestuff(char ***kvms, int nlines);
void dirvalue(char *value);
void fold(char *value);
char *makekey(char *value, int dofold, int dodirsort);
char **keyvaluepair(char *key, char *value);
char *key(char *pair[]);
char *value(char *pair[]);
int keycompare(void *a, void *b);

int sortcount; /* number of fields we're sorting by */
int *fields; /* ordered index of the fields to sort by*/
int (**compares)(void *, void *); /* comparison function for each sort field */
int *folds; /* whether to fold each sort sort field */
int *dirsorts; /* whether dir sort each sort sort field */

int main(int argc, char *argv[])
{
	int i, nlines;
	char *buff, **lines, ***kvps;

	parseargs(argc, argv);
	printf("sortcount: %d\n", sortcount);

	for (i = 0; i < sortcount; i++)
		printf("i:%d, field:%d, fold:%d, dir:%d, fp:%p \n", i,
		       fields[i], folds[i], dirsorts[i], compares[i]);

	if ((nlines = readlines(&buff, &lines)) == LNS_ERROR) {
		printf("input too big to sort\n");
		return 0;
	}

	/*	kvps = malloc(nlines * sizeof(char **));
	for (i = 0; i < nlines; i++) {
		char *key = makekey(lines[i], dofold, dodirsort);
		kvps[i] = keyvaluepair(key, lines[i]);
	}

	quicksort((void **)kvps, 0, nlines - 1, keycompare);

	for (i = 0; i < nlines; i++)
		lines[i] = value(kvps[i]);

	writelines(lines, nlines);

	freelines(buff, lines);
	freestuff(kvps, nlines);*/
	return 0;
}

void parseargs(int argc, char *argv[])
{
	int i;
	int maxsortcount = argc - 1;

	sortcount = 0;

	fields = malloc(maxsortcount * sizeof(int));
	compares = malloc(maxsortcount * sizeof(int (*)(void *, void *)));
	folds = malloc(maxsortcount * sizeof(int));
	dirsorts = malloc(maxsortcount * sizeof(int));

	argv++;
	for (i = 0; i < argc - 1; i++) {
		int field = UNSPEC;
		int numeric = 0;
		int reverse = 0;
		int (*compare)(void *, void *);
		int fold = 0;
		int dirsort = 0;

		char *arg = argv[i];
		if (*arg == '-' && isdigit(*++arg)) {
			field = *arg - '0';
			while (*++arg) {
				if (*arg == 'n')
					numeric = 1;
				if (*arg == 'r')
					reverse = 1;
				else if (*arg == 'f')
					fold = 1;
				else if (*arg == 'd')
					dirsort = 1;
			}
			if (numeric && reverse)
				compare = (int (*)(void *, void *))numcmpr;
			else if (numeric)
				compare = (int (*)(void *, void *))numcmp;
			else if (reverse)
				compare = (int (*)(void *, void *))strcmpr;
			else
				compare = (int (*)(void *, void *))strcmp;
			fields[sortcount] = field;
			compares[sortcount] = compare;
			folds[sortcount] = fold;
			dirsorts[sortcount] = dirsort;
			sortcount++;
		}
	}
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
	return 0; /*(*compare)(key(a), key(b));*/
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

int numcmpr(char *s1, char *s2)
{
	return numcmp(s2, s1);
}

int strcmpr(char *s1, char *s2)
{
	return strcmp(s2, s1);
}
