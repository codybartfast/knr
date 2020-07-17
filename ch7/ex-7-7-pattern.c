/*
 * Exercise 7-7.
 *
 * Modify the pattern finding program of Chapter 5 to take its input from a set
 * of named files or, if no files are named as arguments, from the standard
 * input.  Should the file name be printed when a matching line is found?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

enum errors {
	NO_ERROR = 0,
	ILLEGAL_OPTION,
	NO_PATTERN,
	FILE_OPEN_ERROR,
	FILE_READ_ERROR
};

void parseargs(int argc, char **argv, int *x, int *n, char **ptn, int *fstpath);
static char *chkfgets(char *s, int n, FILE *iop, char *path);
static void close(void);

char *inpath;
FILE *infile;
static char errormsg[MAXLINE];

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	long lineno = 0;
	int i, found = 0; /* no longer returned */

	int except, number, firstpath;
	char *pattern;
	parseargs(argc, argv, &except, &number, &pattern, &firstpath);

	infile = stdin;

	for (i = firstpath; i < argc; i++) {
		printf("file: %s\n", argv[i]);
	}

	while ((chkfgets(line, MAXLINE, infile, inpath)) != NULL) {
		lineno++;
		if ((strstr(line, *argv) != NULL) != except) {
			if (number)
				printf("%ld:", lineno);
			printf("%s", line);
			found++;
		}
	}
	close();
	exit(NO_ERROR);
}

void parseargs(int argc, char *argv[], int *x, int *n, char **ptn, int *fstpath)
{
	char c, *arg;
	int ai = 1;
	for (; ai < argc && (arg = argv[ai])[0] == '-'; ai++) {
		while ((c = *++arg)) {
			switch (c) {
			case 'x':
				*x = 1;
				break;
			case 'n':
				*n = 1;
				break;
			default:
				fprintf(stderr, "find: illegal option %c", c);
				exit(ILLEGAL_OPTION);
			}
		}
	}
	if (ai == argc) {
		fprintf(stderr, "Usage: find -x -n pattern\n");
		exit(NO_PATTERN);
	} else {
		*ptn = argv[ai++];
		*fstpath = ai;
	}
}

FILE *chkfopen(char *path, char *modes)
{
	FILE *fp = fopen(path, modes);
	if (fp != NULL)
		return fp;
	sprintf(errormsg, "error: Failed to open file: '%s'", path);
	perror(errormsg);
	close();
	exit(FILE_OPEN_ERROR);
}

char *chkfgets(char *s, int n, FILE *iop, char *path)
{
	char *r = fgets(s, n, iop);
	if (!ferror(iop))
		return r;
	sprintf(errormsg, "error: Failed to read file: '%s'", path);
	perror(errormsg);
	close();
	exit(FILE_READ_ERROR);
}

void close(void)
{
	if (infile != NULL && infile != stdin)
		fclose(infile);
}
