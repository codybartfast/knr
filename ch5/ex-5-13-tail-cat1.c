/*
 * Category 1
 * ==========
 * Uses:
 * 	- structures (Block) from Chapter 6
 *      - unix read/write from Chapter 8.2
 * 	- error handler from Chapter 8.3
 */

#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_TAILSIZE 10
#define BLOCKSIZE BUFSIZ

typedef struct block {
	char *buff;
	unsigned used;
	unsigned nl_count;
	struct block *next;
	struct block *prev;
} Block;

Block *readfile(Block *, unsigned long nl_target);
Block *newblock(void);
void insertblock(Block *);
int hasfinalnl(Block *);
Block *seekstart(Block *, unsigned long nl_rqstd, int *start);
void writeblock(Block *, int start);
void freeblocks(Block *block);
unsigned long parseargs(int argc, char **argv);
void error(char *fmt, ...);

int block_count = 1;
unsigned long nl_total = 0;

int main(int argc, char *argv[])
{
	unsigned long lines_rqstd = parseargs(argc, argv);
	unsigned long nl_target = lines_rqstd + 1;
	int start;

	Block *lastblock, *block = newblock();
	block->next = block->prev = block;

	if (lines_rqstd == 0)
		return 0;
	lastblock = readfile(block, nl_target);

	if (!hasfinalnl(lastblock))
		nl_target--;
	block = seekstart(lastblock, nl_target, &start);

	writeblock(block, start);
	while (block != lastblock) {
		block = block->next;
		writeblock(block, 0);
	}

	freeblocks(block);
	return 0;
}

Block *readfile(Block *block, unsigned long nl_target)
{
	int nread, i;
	char *p;
	register unsigned nl_count = 0L;

	while (1) {
		/* try to fill the block */
		while (block->used < BLOCKSIZE &&
		       0 < (nread = read(STDIN_FILENO,
					 block->buff + block->used,
					 BLOCKSIZE - block->used))) {
			block->used += nread;
		}
		if (nread < 0)
			error("error reading input.");

		/* count newlines in block */
		for (i = block->used, p = block->buff; i > 0; i--)
			nl_count += (*p++ == '\n');
		block->nl_count = nl_count;
		nl_total += nl_count;

		/* more input? */
		if (nread > 0) {
			/* prepare next block */
			if (nl_total - block->next->nl_count < nl_target)
				insertblock(block);
			block = block->next;
			nl_total -= block->nl_count;
			block->used = 0;
			nl_count = block->nl_count = 0;
		} else {
			return block;
		}
	}
}

Block *newblock(void)
{
	Block *b = malloc(sizeof(Block));

	if (b == NULL)
		error("error: no memory - Block");
	b->buff = malloc(BLOCKSIZE);
	if (b->buff == NULL)
		error("error: no memory - buff");
	b->used = 0;
	b->nl_count = 0;
	return b;
}

void insertblock(Block *block)
{
	Block *inter;

	inter = newblock();
	if (inter == NULL)
		error("error: no memory - inter");
	block_count++;
	block->next->prev = inter;
	inter->next = block->next;
	inter->prev = block;
	block->next = inter;
}

int hasfinalnl(Block *block)
{
	char lastchar = 0;

	if (block->used)
		lastchar = *(block->buff + block->used - 1);
	else if (block_count > 1)
		lastchar = *(block->prev->buff + BLOCKSIZE - 1);
	return lastchar == '\n';
}

Block *seekstart(Block *block, unsigned long nl_rqstd, int *start)
{
	int blkcount = block_count;
	char *p;

	/* find block with first line or first block */
	while (block->nl_count < nl_rqstd && --blkcount) {
		nl_rqstd -= block->nl_count;
		block = block->prev;
	}

	/* fewer lines than requested? */
	if (block->nl_count < nl_rqstd) {
		*start = 0;
	} else {
		p = block->buff + block->used;
		while (nl_rqstd > 0)
			if (*--p == '\n')
				nl_rqstd--;
		*start = (p - block->buff) + 1;
	}
	return block;
}

void writeblock(Block *block, int start)
{
	int nwrote;
	nwrote = write(STDOUT_FILENO, block->buff + start, block->used - start);
	if (nwrote < 0)
		error("problem writing to <stdout>");
}

void freeblocks(Block *block)
{
	Block *first = block;

	do {
		Block *next = block->next;
		free(block->buff);
		free(block);
		block = next;
	} while (block != first);
}

unsigned long parseargs(int argc, char **argv)
{
	char c, *ptr;
	unsigned int digit;
	unsigned long n = 0;
	unsigned long max = ULONG_MAX;

	if (argc <= 1)
		n = DEFAULT_TAILSIZE;
	else if (argc == 2 && *argv[1] == '-') {
		ptr = argv[1] + 1;
		if (*ptr == '\0') /* match behaviour of linux tail */
			n = DEFAULT_TAILSIZE;
		while ((c = *ptr++) != '\0') {
			if (!isdigit(c))
				error("Expected decimal integer: %s", argv[1]);
			if (n > (max / 10))
				error("Argument too large, max: %lu", max);
			n *= 10;
			digit = (c - '0');
			if (digit > max - n)
				error("Argument too large, max: %lu", max);
			n += digit;
		}
	} else if (argc == 2)
		error("Expected argument to begin with '-'");
	else
		error("Expected at most one argument, got %d.", argc - 1);
	return n;
}

void error(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}
