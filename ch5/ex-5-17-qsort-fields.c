/* Exercise 5-17
 *
 * Add a field-handling capability, so sorting may be done on fields within
 * lines, each field sorted according to an independent set of options.  (The
 * index for this book was sorted with -df for the index category and -n for the
 * page numbers.)
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lines.h"
#include "quicksort.h"

int comparefields(void *s1, void *s2);

void parseargs(int argc, char *argv[]);
char **makefieldset(char *line);

char *field(void *, int sortidx);
char *line(char *set[]);
char *fieldspace(char *set[]);
void fold(char *value);
void dirvalue(char *value);
int strcmpr(char *, char *);
int numcmp(char *, char *);
int numcmpr(char *, char *);
void freestuff(char ***kvms, int nlines);

int sortcount; /* number of fields we're sorting by */
int maxfield; /* highest field index to sort */
int *sortidxs; /* fields to sort by ordered by priority */
int *usedfields; /* fields to soft by ordered by column */
int (**compares)(void *, void *); /* comparison function for each sort field */
int *folds; /* whether to fold each sort sort field */
int *dirsorts; /* whether dir sort each sort sort field */
char ***fieldsets; /* line with the fields to sort by modified with -f, -d */

int main(int argc, char *argv[])
{
	int i, nlines;
	char *buff, **lines, ***fieldsets;

	parseargs(argc, argv);

	if ((nlines = readlines(&buff, &lines)) == LNS_ERROR) {
		printf("input too big to sort\n");
		return 0;
	}

	fieldsets = malloc(nlines * sizeof(char **));
	for (i = 0; i < nlines; i++)
		fieldsets[i] = makefieldset(lines[i]);

	quicksort((void **)fieldsets, 0, nlines - 1,
		  (int (*)(void *, void *))comparefields);

	for (i = 0; i < nlines; i++)
		lines[i] = line(fieldsets[i]);

	writelines(lines, nlines);

	freestuff(fieldsets, nlines);
	freelines(buff, lines);
	return 0;
}

int comparefields(void *fields1, void *fields2)
{
	int i, rslt = 0;

	for (i = 0; i < sortcount && rslt == 0; i++)
		rslt = (*compares[i])(field(fields1, i), field(fields2, i));
	return rslt;
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
		int fieldidx;
		int numeric = 0;
		int reverse = 0;
		int (*compare)(void *, void *);
		int fold = 0;
		int dirsort = 0;

		char *arg = argv[i];
		if (*arg == '-' && isdigit(*++arg)) {
			fieldidx = atoi(arg) - 1;
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
			maxfield = fieldidx > maxfield ? fieldidx : maxfield;
			sortidxs[sortcount] = fieldidx;
			compares[sortcount] = compare;
			folds[sortcount] = fold;
			dirsorts[sortcount] = dirsort;
			sortcount++;
		}
	}
}

char **makefieldset(char *line)
{
	char *fieldspace = malloc((strlen(line) + 1) * sizeof(char));
	char *chr, *field;
	int fieldidx, i;

	char **tmpfields = malloc((maxfield + 1) * sizeof(char *));
	char **set = malloc((2 + sortcount) * sizeof(char *));

	strcpy(fieldspace, line);
	set[0] = line;
	set[1] = fieldspace;

	field = fieldspace;
	fieldidx = 0;
	for (chr = fieldspace; *chr; chr++) {
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
	free(tmpfields);
	return set;
}

char *field(void *set, int idx)
{
	return *(((char **)set) + 2 + idx);
}

char *line(char *set[])
{
	return set[0];
}

char *fieldspace(char *set[])
{
	return set[1];
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

int strcmpr(char *s1, char *s2)
{
	return strcmp(s2, s1);
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

void freestuff(char ***kvps, int nlines)
{
	int i;
	for (i = 0; i < nlines; i++) {
		free(fieldspace(kvps[i]));
		free(kvps[i]);
	}
	free(fieldsets);

	free(sortidxs);
	free(compares);
	free(folds);
	free(dirsorts);
}
