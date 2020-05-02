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

#define INITIAL_BUFFSIZE (18)
#define REQSTD_DEFAULT 10

enum { OK, ERROR };

unsigned long int buffsize;
unsigned long int maxbuffsize = ULONG_MAX;
char *buff, *buffptr, *bufflimit;
long linessize;
char **lines, **first, **line, **lineslimit;

int readlines(void);
void writelines(void);
void initialize(void);
int growbuff(void);
long getreqstd(int argc, char **argv);

int main(int argc, char **argv)
{
	linessize = getreqstd(argc, argv) + 1;
	buffsize = INITIAL_BUFFSIZE;
	initialize();

	if (readlines() == OK)
		writelines();

	/* 	free(lines);
	free(buff); */
	return 0;
}

int readlines(void)
{
	char c;

	while ((c = getchar()) != EOF) {
		*buffptr = c;
		buffptr++;
		if (buffptr == bufflimit)
			buffptr = buff;
		if (buffptr == *first) {
			if (growbuff() != OK) {
				return 1;
			}
		}
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
	if (*first <= buffptr) {
		for (ptr = *first; ptr < buffptr; ptr++)
			putchar(*ptr);
	} else {
		for (ptr = *first; ptr < bufflimit; ptr++)
			putchar(*ptr);
		for (ptr = buff; ptr < buffptr; ptr++)
			putchar(*ptr);
	}
}

void initialize(void)
{
	buff = (char *)malloc(buffsize * sizeof(char));
	buffptr = buff;
	bufflimit = buff + buffsize;
	lines = (char **)malloc(linessize * sizeof(char *));
	first = line = lines;
	lineslimit = lines + linessize;
	*line = buffptr;
}

int growbuff(void)
{
	unsigned long tbuffsize = buffsize;
	char *tbuff = buff, *tbuffptr = buffptr, *tbufflimit = bufflimit;
	char *bptr;
	char **tlines = lines, **tfirst = first, **tline = line,
	     **tlineslimit = lineslimit;
	char **tlptr, **lptr;

	printf("buffsize %ld -> ", buffsize);
	if (buffsize == maxbuffsize) {
		printf("error: Cannot increase buffer size (%ld char)",
		       buffsize);
	}
	buffsize = (buffsize > maxbuffsize / 2) ? maxbuffsize : (buffsize * 2);
	printf("%ld\n", buffsize);

	initialize();

	if (*tfirst < tbuffptr) {
		for (bptr = *tfirst; bptr < tbuffptr; buffptr++, bptr++)
			       *buffptr = *bptr;
	} else {
		for (bptr = *tfirst; bptr < tbufflimit; buffptr++, bptr++)
			       *buffptr = *bptr;
		for (bptr = tbuff; bptr < tbuffptr; buffptr++, bptr++)
			       *buffptr = *bptr;
	}

	for (tlptr = tfirst, lptr = lines; tlptr < tlineslimit;
	     tlptr++, lptr++) {
		if (*tlptr == NULL)
			break;
		*lptr = buff + ((*tlptr >= *tfirst) ?
					*tlptr - *tfirst :
					((tbufflimit - 0) - *tfirst) +
						(*tlptr - tbuff));
		if (tlptr == tfirst)
			first = lptr;
		else if (tlptr == tline)
			line = lptr;
	}	
	for (tlptr = tlines, lptr = lptr; tlptr < tfirst;
	     tlptr++, lptr++) {
		if (*tlptr == NULL)
			break;
		*lptr = buff + ((*tlptr >= *tfirst) ?
					*tlptr - *tfirst :
					((tbufflimit - 0) - *tfirst) +
						(*tlptr - tbuff));
		if (tlptr == tfirst)
			first = lptr;
		else if (tlptr == tline)
			line = lptr;
	}

	buffptr = buff + tbuffsize;
	return OK;
}

long getreqstd(int argc, char **argv)
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
