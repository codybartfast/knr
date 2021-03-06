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
#define DEFAULT_TAILSIZE 10

enum { OK = 0, ERROR = 1 };

/* Single buffer to store characters, loops back to start on reaching limit */
unsigned long buffsize;
unsigned long maxbuffsize = ULONG_MAX;
char *buff, *cursor, *bufflimit;

/* max number of lines to print */
unsigned long tailsize;
/* pointers to the start of lines in the buffer, loops back start at limit */
char **lines, **firstln, **currln, **lineslimit;

int readlines(void);
void writelines(void);
int init_buff(unsigned long);
int init_lines(void);
int enlargebuff(void);
int parseargs(int argc, char **argv);

int main(int argc, char **argv)
{
	if (parseargs(argc, argv) != OK)
		return 0;
	if (tailsize == 0)
		return 0;
	if (init_buff(INITIAL_BUFFSIZE) != OK)
		return 0;
	if (init_lines() != OK) {
		free(buff);
		return 0;
	}

	if (readlines() == OK)
		writelines();

	free(lines);
	free(buff);
	return 0;
}

int readlines(void)
{
	int c, prevc = '\0';

	while ((c = getchar()) != EOF) {
		if (prevc == '\n') {
			if (++currln == lineslimit)
				currln = lines;
			*currln = cursor;
			if (currln == firstln)
				if (++firstln == lineslimit)
					firstln = lines;
		}
		*cursor = prevc = c;
		if (++cursor == bufflimit)
			cursor = buff;
		if (cursor == *firstln) {
			if (enlargebuff() != OK)
				return ERROR;
		}
	}
	return OK;
}

void writelines(void)
{
	char *bptr;

	if (*firstln <= cursor) {
		for (bptr = *firstln; bptr < cursor; bptr++)
			putchar(*bptr);
	} else {
		for (bptr = *firstln; bptr < bufflimit; bptr++)
			putchar(*bptr);
		for (bptr = buff; bptr < cursor; bptr++)
			putchar(*bptr);
	}
}

int init_buff(unsigned long size)
{
	buffsize = size;
	buff = malloc(buffsize * sizeof(char));
	if (buff == NULL) {
		printf("error: Insufficient memory (allocating buff)\n");
		return ERROR;
	}
	cursor = buff;
	bufflimit = buff + buffsize;
	return OK;
}

int init_lines(void)
{
	lines = malloc(tailsize * sizeof(char *));
	if (lines == NULL) {
		printf("error: Insufficient memory (allocating lines)\n");
		return ERROR;
	}
	firstln = currln = lines;
	*lines = buff;
	lineslimit = lines + tailsize;
	return OK;
}

int enlargebuff(void)
{
	unsigned long used;
	char *bptr, **lptr;
	ptrdiff_t offset;

	/* copy info about current buffer before it's replaced */
	char *pbuff = buff, *pcursor = cursor, *pbufflimit = bufflimit;
	char *pfirstchr = *firstln;
	ptrdiff_t pfirst2limit = pbufflimit - pfirstchr;

	if (buffsize == maxbuffsize) {
		printf("error: Cannot increase buffer size beyond %ld\n",
		       buffsize);
		return ERROR;
	}

	/* replace buff */
	buffsize = buffsize > maxbuffsize / 4 ? maxbuffsize : buffsize * 4;
	if (init_buff(buffsize) != OK) {
		return ERROR;
	}

	/* copy data from previous buff to new buff */
	if (pfirstchr < pcursor) {
		for (bptr = pfirstchr; bptr < pcursor;)
			*cursor++ = *bptr++;
		used = pcursor - pfirstchr;
	} else {
		for (bptr = pfirstchr; bptr < pbufflimit;)
			*cursor++ = *bptr++;
		for (bptr = pbuff; bptr < pcursor;)
			*cursor++ = *bptr++;
		used = pfirst2limit + (pcursor - pbuff);
	}

	/* update line pointers to point to lines' new positions in new buff,
	   may copy uninitialised values */
	for (lptr = lines; lptr < lineslimit; lptr++) {
		offset = *lptr >= pfirstchr ? *lptr - pfirstchr :
					      pfirst2limit + (*lptr - pbuff);
		*lptr = buff + offset;
	}

	/* set cursor for new buff */
	cursor = buff + used;

	free(pbuff);
	return OK;
}

int parseargs(int argc, char **argv)
{
	char c, *ptr;
	unsigned int digit;
	unsigned long n = 0;
	unsigned long max = ULONG_MAX;

	if (argc <= 1) {
		tailsize = DEFAULT_TAILSIZE;
	} else if (argc == 2 && *argv[1] == '-') {
		ptr = argv[1] + 1;
		if (*ptr == '\0') /* match behaviour of linux tail */
			n = DEFAULT_TAILSIZE;
		while ((c = *ptr++) != '\0') {
			if (!isdigit(c)) {
				printf("error: Expected decimal integer: %s\n",
				       argv[1] + 1);
				return ERROR;
			}
			if (n > (max / 10)) {
				printf("error: Argument too large, max: %lu\n",
				       max);
				return ERROR;
			}
			n *= 10;
			digit = (c - '0');
			if (digit > max - n) {
				printf("error: Argument too large, max: %lu\n",
				       max);
				return ERROR;
			}
			n += digit;
		}
		tailsize = n;
	} else if (argc == 2) {
		printf("error: Expected argument to begin with '-'\n");
		return ERROR;
	} else {
		printf("error: Expected at most one argument, got %d.\n",
		       argc - 1);
		return ERROR;
	}
	return OK;
}
