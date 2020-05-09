/* Exercise 5-17
 *
 * Add a field-handling capability, so sorting may be done on fields within
 * lines, each field sorted according to an independent set of options.  (The
 * index for this book was sorted with -df for the index category and -n for the
 * page numbers.)
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
int issortfield(int idx);
char **keyset(char *row);
char *key(char *set[]);
char *value(char *set[]);
char *sortfield(char *set[], int sortidx);
int keycompare(void *a, void *b);

int sortcount; /* number of fields we're sorting by */
int maxfield; /* highest field index to sort */
int *sortidxs; /* fields to sort by ordered by priority */
int *usedfields; /* fields to soft by ordered by column */
int (**compares)(void *, void *); /* comparison function for each sort field */
int *folds; /* whether to fold each sort sort field */
int *dirsorts; /* whether dir sort each sort sort field */

int main(int argc, char *argv[])
{
	int i, nlines;
	char *buff, **lines, ***keysets;

	parseargs(argc, argv);
	printf("sortcount: %d\n", sortcount);
	printf("maxfield: %d\n", maxfield);

	for (i = 0; i < sortcount; i++)
		printf("i:%d, field:%d, fold:%d, dir:%d, fp:%%p \n", i,
		       sortidxs[i], folds[i], dirsorts[i] /*, sortidxs[i] */);

	if ((nlines = readlines(&buff, &lines)) == LNS_ERROR) {
		printf("input too big to sort\n");
		return 0;
	}

	keysets = malloc(nlines * sizeof(char **));
	for (i = 0; i < nlines; i++) {
		keysets[i] = keyset(lines[i]);
	}
	/*
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

	sortidxs = malloc(maxsortcount * sizeof(int));
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
			field = atoi(arg) - 1;
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
			maxfield = field > maxfield ? field : maxfield;
			sortidxs[sortcount] = field;
			compares[sortcount] = compare;
			folds[sortcount] = fold;
			dirsorts[sortcount] = dirsort;
			sortcount++;
		}
	}
}

char **keyset(char *row)
{
	long len = strlen(row);
	char *keysstr = malloc((len + 1) * sizeof(char));
	char *chr, *field;
	int fieldidx, i;

	char **tmpfields = malloc((maxfield + 1) * sizeof(char *));
	char **set = malloc((2 + sortcount) * sizeof(char *));

	strcpy(keysstr, row);
	set[0] = row;
	set[1] = keysstr;

	field = keysstr;
	fieldidx = 0;
	for (chr = keysstr; *chr; chr++) {
		if (*chr == '\t' || *chr == '\n') {
			*chr = '\0';
			if (fieldidx <= maxfield)
				tmpfields[fieldidx] = field;
			field = chr + 1;
			fieldidx++;
		}
	}
	for (i = 0; i < sortcount; i++) {
		field = tmpfields[sortidxs[i]];
		if (folds[i])
			fold(field);
		if (dirsorts[i])
			dirvalue(field);
		set[2 + i] = field;
	}

	printf("\nrow:    %s", row);
	printf("fields: ");
	for (i = 0; i <= maxfield; i++)
		printf("%s, ", tmpfields[i]);
	printf("\nkeys:   ");
	for (i = 0; i < sortcount; i++)
		printf("%s, ", set[2 + i]);
	printf("\n");

	return set;
}

char *key(char *set[])
{
	return set[0];
}

char *value(char *set[])
{
	return set[1];
}

char *sortfield(char *set[], int idx)
{
	return set[2 + idx];
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
