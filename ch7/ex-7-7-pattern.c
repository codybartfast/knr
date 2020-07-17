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

static void parseargs(int argc, char **argv, int *x, int *n, char **ptn,
		      int *fstpath);
static int find(FILE *file, char *path, char *pattern, int except, int number);
static FILE *chkfopen(char *file, char *modes);
static char *chkfgets(char *s, int n, FILE *iop, char *path);
static void close(void);

static char errormsg[MAXLINE];

int main(int argc, char *argv[])
{
	int i, except, number, firstpath, found = 0;
	char *pattern, *path, *modes = "r";
	FILE *file;

	parseargs(argc, argv, &except, &number, &pattern, &firstpath);

	if (firstpath < argc) {
		for (i = firstpath; i < argc; i++) {
			path = argv[i];
			file = chkfopen(path, modes);
			found += find(file, path, pattern, except, number);
		}
	} else {
		found = find(stdin, "<stdin>", pattern, except, number);
	}

	close();
	exit(NO_ERROR);
}

int find(FILE *file, char *path, char *pattern, int except, int number)
{
	char line[MAXLINE];
	long lineno = 0, found = 0;

	while ((chkfgets(line, MAXLINE, file, path)) != NULL) {
		lineno++;
		if ((strstr(line, pattern) != NULL) != except) {
			if (number)
				printf("%ld:", lineno);
			printf("%s", line);
			found++;
		}
	}
	return found;
}

void parseargs(int argc, char *argv[], int *x, int *n, char **ptn, int *fstpath)
{
	char c, *arg;
	int ai;
	for (ai = 1; ai < argc && (arg = argv[ai])[0] == '-'; ai++) {
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
	/* 	if (infile != NULL && infile != stdin)
		fclose(infile); */
}
