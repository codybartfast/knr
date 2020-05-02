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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BUFFSIZE (1 << 10)
#define REQSTD_DEFAULT 10

enum { OK, ERROR };

unsigned long int buffsize;
unsigned long int maxbuffsize = ULONG_MAX;
char *buff, *buffptr, *bufflimit;
long tailsize;
char **lines, **first, **line, **lineslimit;

int readlines(void);
void writelines(void);
void init_buff(unsigned long);
int growbuff(void);
long gettailsize(int argc, char **argv);

int main(int argc, char **argv)
{
	if ((tailsize = gettailsize(argc, argv)) == 0)
		return 0;
	init_buff(INITIAL_BUFFSIZE);
	lines = (char **)malloc(tailsize * sizeof(char *));
	lineslimit = lines + tailsize;
	first = line = lines;
	*lines = buff;

	if (readlines() == OK)
		writelines();

	free(lines);
	free(buff);
	return 0;
}

int readlines(void)
{
	char c;
	int afternl = 0;

	while ((c = getchar()) != EOF) {
		*buffptr = c;
		if (afternl) {
			if (++line == lineslimit)
				line = lines;
			if (line == first)
				if (++first == lineslimit)
					first = lines;
			*line = buffptr;
		}
		if (buffptr++ == bufflimit)
			buffptr = buff;
		if (buffptr == *first) {
			if (growbuff() == ERROR)
				return ERROR;
		}
		afternl = (c == '\n');
	}
	return OK;
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

void init_buff(unsigned long size)
{
	buffsize = size;
	buff = (char *)malloc(buffsize * sizeof(char));
	buffptr = buff;
	bufflimit = buff + buffsize;
}

int growbuff(void)
{
	unsigned long used;
	char *bptr, **lptr;
	ptrdiff_t offset;

	char *pbuff = buff, *pbuffptr = buffptr, *pbufflimit = bufflimit;
	char *pfirst = *first;
	ptrdiff_t pstart2limit = pbufflimit - pfirst;

	if (buffsize == maxbuffsize) {
		printf("error: Cannot increase buffer size beyond %ld",
		       buffsize);
		return ERROR;
	}

	init_buff(buffsize > maxbuffsize / 4 ? maxbuffsize : buffsize * 4);

	if (pfirst < pbuffptr) {
		for (bptr = pfirst; bptr < pbuffptr;)
			*buffptr++ = *bptr++;
		used = pbuffptr - pfirst;
	} else {
		for (bptr = pfirst; bptr < pbufflimit;)
			*buffptr++ = *bptr++;
		for (bptr = pbuff; bptr < pbuffptr;)
			*buffptr++ = *bptr++;
		used = pstart2limit + (pbuffptr - pbuff);
	}

	for (lptr = lines; lptr < lineslimit && *lptr != NULL; lptr++) {
		offset = *lptr >= pfirst ? *lptr - pfirst :
					   pstart2limit + (*lptr - pbuff);
		*lptr = buff + offset;
	}
	buffptr = buff + used;

	free(pbuff);
	return OK;
}

long gettailsize(int argc, char **argv)
{
	char c, *ptr;
	int n = 0;

	if (argc == 2 && *argv[1] == '-') {
		ptr = argv[1] + 1;
		while ((c = *ptr++) != '\0' && isdigit(c))
			n = (n * 10) + (c - '0');
		return n;
	} else {
		return REQSTD_DEFAULT;
	}
}
