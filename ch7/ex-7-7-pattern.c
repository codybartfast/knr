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

enum errors { NO_ERROR = 0, FILE_OPEN_ERROR, FILE_READ_ERROR };

static char *chkfgets(char *s, int n, FILE *iop, char *path);
static void close(void);

char *inpath;
FILE *infile;
static char errormsg[MAXLINE];

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	long lineno = 0;
	int c, except = 0, number = 0;
	int found = 0; /* not returned */
	infile = stdin;

	while (--argc > 0 && (*++argv)[0] == '-') {
		while ((c = *++argv[0])) {
			switch (c) {
			case 'x':
				except = 1;
				break;
			case 'n':
				number = 1;
				break;
			default:
				printf("find illegal option %c\n", c);
				argc = 0;
				found = -1;
				break;
			}
		}
	}
	if (argc != 1) {
		printf("Usage: find -x -n pattern\n");
	} else {
		while ((chkfgets(line, MAXLINE, infile, inpath)) != NULL) {
			lineno++;
			if ((strstr(line, *argv) != NULL) != except) {
				if (number)
					printf("%ld:", lineno);
				printf("%s", line);
				found++;
			}
		}
	}
	close();
	exit(NO_ERROR);
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
