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
	NOTHING_FOUND,
	ILLEGAL_OPTION,
	NO_PATTERN,
	FILE_OPEN_ERROR,
	FILE_READ_ERROR
};

static void parseargs(int argc, char *argv[], int *except, int *number,
		      int *name, char **pattern, int *firstpath);
static int find(FILE *file, char *path, char *pattern, int except, int number,
		int name);
static FILE *chkfopen(char *file, char *modes);
static char *chkfgets(char *s, int n, FILE *iop, char *path);

static char errormsg[MAXLINE];

int main(int argc, char *argv[])
{
	int i, except, lnnum, name, paths, found = 0;
	char *pattern, *path, *modes = "r";
	FILE *file;

	parseargs(argc, argv, &except, &lnnum, &name, &pattern, &paths);

	if (paths < argc) {
		for (i = paths; i < argc; i++) {
			path = argv[i];
			file = chkfopen(path, modes);
			found += find(file, path, pattern, except, lnnum, name);
			fclose(file);
		}
	} else {
		found = find(stdin, "<stdin>", pattern, except, lnnum, name);
	}
	exit(found ? NO_ERROR : NOTHING_FOUND);
}

int find(FILE *file, char *path, char *pattern, int except, int number,
	 int name)
{
	char line[MAXLINE];
	long lineno = 0, found = 0;

	while ((chkfgets(line, MAXLINE, file, path)) != NULL) {
		lineno++;
		if ((strstr(line, pattern) != NULL) != except) {
			if (name)
				printf("%s:", path);
			if (number)
				printf("%ld:", lineno);
			printf("%s", line);
			found++;
		}
	}
	return found;
}

void parseargs(int argc, char **argv, int *except, int *number, int *name,
	       char **pattern, int *firstpath)
{
	char c, *arg;
	int ai;

	*except = *number = *name = 0;
	for (ai = 1; ai < argc && (arg = argv[ai])[0] == '-'; ai++) {
		while ((c = *++arg)) {
			switch (c) {
			case 'x':
				*except = 1;
				break;
			case 'n':
				*number = 1;
				break;
			case 'f':
				*name = 1;
				break;
			default:
				fprintf(stderr, "find: illegal option %c", c);
				exit(ILLEGAL_OPTION);
			}
		}
	}
	if (ai == argc) {
		fprintf(stderr, "Usage: find -x -n -f pattern\n");
		exit(NO_PATTERN);
	} else {
		*pattern = argv[ai++];
		*firstpath = ai;
	}
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
