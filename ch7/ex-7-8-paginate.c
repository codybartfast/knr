/*
 * Exercice 7-8.
 *
 * Write a program to print a set of files, starting each new one one new page,
 * with a title and a running page count for each file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGELENGTH 20
#define MAXLINE 1000

enum error { NO_ERROR = 0, FILE_OPEN_ERROR, FILE_READ_ERROR };

void printfile(char *);
int printpages(char *path, FILE *file, int pagenum);
static FILE *chkfopen(char *file, char *modes);
static char *chkfgets(char *s, int n, FILE *iop, char *path);

static char errormsg[MAXLINE];

int main(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++)
		printfile(argv[i]);

	exit(NO_ERROR);
}

void printfile(char *path)
{
	int lnidx, i;

	if ((lnidx = printpages(path, chkfopen(path, "r"), 1)))
		/* if not at top of page print '\n's to get to next page */
		for (i = PAGELENGTH - lnidx; i; i--)
			putchar('\n');
}

int printpages(char *path, FILE *file, int pagenum)
{
	static char line[MAXLINE];
	int lnidx = 2;

	/* Don't print blank pages! */
	if (chkfgets(line, MAXLINE, file, path) == NULL)
		return 0;

	printf("# File: %-60.60s   Page %4d\n\n", path, pagenum);
	do {
		printf("%s", line);
	} while (++lnidx < PAGELENGTH && chkfgets(line, MAXLINE, file, path));

	/* Add final \n if the file did not end with \n */
	if (line[strlen(line) - 1] != '\n')
		putchar('\n');

	return lnidx < PAGELENGTH ? lnidx : printpages(path, file, pagenum + 1);
}

FILE *chkfopen(char *path, char *modes)
{
	FILE *fp = fopen(path, modes);
	if (fp != NULL)
		return fp;
	sprintf(errormsg, "error: Failed to open file: '%s'", path);
	perror(errormsg);
	exit(FILE_OPEN_ERROR);
}

char *chkfgets(char *s, int n, FILE *iop, char *path)
{
	char *r = fgets(s, n, iop);
	if (!ferror(iop))
		return r;
	sprintf(errormsg, "error: Failed to read file: '%s'", path);
	perror(errormsg);
	fclose(iop);
	exit(FILE_READ_ERROR);
}
