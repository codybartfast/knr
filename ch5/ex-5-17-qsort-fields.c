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

#define VOIDCOMP int (*)(void *, void *)
#define CHARCOMP int (*)(char *, char *)

int compare(char **s1, char **s2);

void parseargs(int argc, char *argv[]);
char **makefieldset(char *line);

char *field(void *, int sortidx);
char *line(char *set[]);
char *allfields(char *set[]);
void fold(char *value);
void dirsort(char *value);
int strcmpr(char *, char *);
int numcmp(char *, char *);
int numcmpr(char *, char *);
void freestuff(char ***kvms, int nlines);

int *sortidxs; /* indexes of fields to sort by, ordered by priority */
int nsortflds; /* number of fields we're sorting by */
int maxsortidx; /* highest index of the fields we're sorting by */
int (**compares)(char *, char *); /* comparison function for each sort field */
int *folds; /* whether to fold each sort field */
int *dirsorts; /* whether to use dir sort for each sort field */

char ***fieldsets; /* sort by fields of lines with -f, -d applied if needed */

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

	quicksort((void **)fieldsets, 0, nlines - 1, (VOIDCOMP)compare);

	for (i = 0; i < nlines; i++)
		lines[i] = line(fieldsets[i]);

	writelines(lines, nlines);

	freestuff(fieldsets, nlines);
	freelines(buff, lines);
	return 0;
}

int compare(char **sortflds1, char **sortflds2)
{
	int i, rslt = 0;

	for (i = 0; i < nsortflds && rslt == 0; i++)
		rslt = (*compares[i])(sortflds1[i], sortflds2[i]);
	return rslt;
}

void parseargs(int argc, char *argv[])
{
	int i;
	int maxsortcount = argc - 1;

	nsortflds = 0;
	sortidxs = malloc(maxsortcount * sizeof(int));
	compares = malloc(maxsortcount * sizeof(CHARCOMP));
	folds = malloc(maxsortcount * sizeof(int));
	dirsorts = malloc(maxsortcount * sizeof(int));

	argv++;
	for (i = 0; i < argc - 1; i++) {
		char *arg = argv[i];

		int fieldidx;
		int numeric = 0;
		int reverse = 0;
		int (*compare)(char *, char *);
		int fold = 0;
		int dirsort = 0;

		if (*arg == '-' && isdigit(*++arg)) {
			fieldidx = atoi(arg) - 1;
			while (*++arg) {
				if (*arg == 'n')
					numeric = 1;
				else if (*arg == 'r')
					reverse = 1;
				else if (*arg == 'f')
					fold = 1;
				else if (*arg == 'd')
					dirsort = 1;
			}
			if (numeric && reverse)
				compare = numcmpr;
			else if (numeric)
				compare = numcmp;
			else if (reverse)
				compare = strcmpr;
			else
				compare = (CHARCOMP)strcmp;
			maxsortidx =
				fieldidx > maxsortidx ? fieldidx : maxsortidx;
			sortidxs[nsortflds] = fieldidx;
			compares[nsortflds] = compare;
			folds[nsortflds] = fold;
			dirsorts[nsortflds] = dirsort;
			nsortflds++;
		}
	}
}

char **makefieldset(char *line)
{
	char *allfields = malloc((strlen(line) + 1) * sizeof(char));
	char *chr, *field;
	int fieldidx, i;

	char **tmpfields = malloc((maxsortidx + 2) * sizeof(char *));
	char **fieldset = malloc((2 + nsortflds) * sizeof(char *));

	strcpy(allfields, line);
	fieldset[nsortflds] = line;
	fieldset[nsortflds + 1] = allfields;

	/* split line into fields and create temporary pointer for each field */
	field = allfields;
	fieldidx = 0;
	for (chr = allfields; *chr; chr++) {
		if (*chr == '\t' || *chr == '\n') {
			*chr = '\0';
			/*maxsortidx + 1 so last sort field is terminated */
			if (fieldidx <= maxsortidx + 1)
				tmpfields[fieldidx] = field;
			field = chr + 1;
			fieldidx++;
		}
	}

	/* ptr to fields to sort by in order, apply -f and-d if needed */
	for (i = 0; i < nsortflds; i++) {
		field = tmpfields[sortidxs[i]];
		if (folds[i])
			fold(field);
		if (dirsorts[i])
			dirsort(field);
		fieldset[i] = field;
	}
	free(tmpfields);
	return fieldset;
}

char *line(char *set[])
{
	return set[nsortflds];
}

char *allfields(char *set[])
{
	return set[nsortflds + 1];
}

void fold(char *value)
{
	while ((*value = toupper(*value)))
		value++;
}

void dirsort(char *value)
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
		free(allfields(kvps[i]));
		free(kvps[i]);
	}
	free(fieldsets);

	free(sortidxs);
	free(compares);
	free(folds);
	free(dirsorts);
}
