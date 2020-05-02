/*
 * Exercise 5-13
 *
 * Write the program tail, which prints the last n lines of its input.  By
 * default, n is 10, let us say, but it can be changed by an optional
 * argument, so that
 *
 * 	tail -n
 *
 * prints the last n lines.  The program should behave rationally no matter
 * how unreasonable the input or the value of n.  Write the program so it
 * makes the best use of available storage; lines should be stored as in the
 * sorting program of Section 5.6, not in a two-dimensional array of fixed
 * size.
 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BUFFSIZE (1 << 10)
#define REQSTD_DEFAULT 10

enum { OK, ERROR };

unsigned long int buffsize;
char *buff, *buffptr, *bufflimit;
long tailsize;
char **lines, **first, **line, **lineslimit;

int readlines(void);
void writelines(void);
void initialize(void);
long gettailsize(int argc, char **argv);

int main(int argc, char **argv)
{
	tailsize = gettailsize(argc, argv) + 1;
	buffsize = INITIAL_BUFFSIZE;
	initialize();

	if (readlines() == OK)
		writelines();

	free(lines);
	free(buff);
	return 0;
}

int readlines(void)
{
	char c;

	while ((c = getchar()) != EOF) {
		*buffptr++ = c;
		if (buffptr == *first) {
			printf("error: Buffer too small (%ld char)\n",
			       buffsize);
			return 1;
		}
		if (buffptr == bufflimit)
			buffptr = buff;
		if (c == '\n') {
			if (++line == lineslimit)
				line = lines;
			if (line == first)
				if (++first == lineslimit)
					first = lines;
			*line = buffptr;
		}
	}
	return 0;
}

void writelines(void)
{
	char *ptr;
	if (*first <= *line) {
		for (ptr = *first; ptr < *line; ptr++)
			putchar(*ptr);
	} else {
		for (ptr = *first; ptr < bufflimit; ptr++)
			putchar(*ptr);
		for (ptr = buff; ptr < *line; ptr++)
			putchar(*ptr);
	}
}

void initialize(void)
{
	buff = (char *)malloc(buffsize * sizeof(char));
	buffptr = buff;
	bufflimit = buff + buffsize;
	lines = (char **)malloc(tailsize * sizeof(char *));
	first = line = lines;
	lineslimit = lines + tailsize;
	*line = buffptr;
}

long gettailsize(int argc, char **argv)
{
	if (argc == 2 && *argv[1] == '-') {
		char c, *ptr;
		int n = 0;
		ptr = argv[1] + 1;
		while ((c = *ptr++) != '\0' && isdigit(c))
			n = (n * 10) + (c - '0');
		return n;
	} else {
		return REQSTD_DEFAULT;
	}
}
