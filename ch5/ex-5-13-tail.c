#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE BUFSIZ

typedef struct block {
	char *buff;
	int used;
	int nl_count;
	struct block *next;
	struct block *prev;
} Block;

Block *readfile(Block *, int nl_target);
int readblock(Block *block);
Block *newblock(void);
void insertblock(Block *);
int finalnl(Block *);
Block *seekstart(Block *, int nl_rqstd, int *start);
void error(char *fmt, ...);

int block_count = 1;
int nl_total = 0;

int main(void)
{
	int lines_rqstd = 10;
	int nl_target = lines_rqstd + 1;
	int start;

	Block *lastblock, *block = newblock();
	block->next = block->prev = block;

	lastblock = readfile(block, nl_target);

	if (!finalnl(lastblock))
		nl_target--;

	block = seekstart(lastblock, nl_target, &start);

	write(STDOUT_FILENO, block->buff + start, block->used - start);

	while (block != lastblock) {
		block = block->next;
		write(STDOUT_FILENO, block->buff, block->used);
	}
	return 0;
}

Block *readfile(Block *block, int nl_target)
{
	int nread, i;
	char *p;
	register int nl_count = 0;

	while (1) {
		/* Invariants?
		   - 'block' is free to be filled
		   - not encountered EOF */

		/* try to fill the block */
		while (block->used < BUFSIZE &&
		       0 < (nread = read(STDIN_FILENO,
					 block->buff + block->used,
					 BUFSIZE - block->used))) {
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
	b->buff = malloc(BUFSIZE);
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

int finalnl(Block *block)
{
	char lastchar = 0;

	if (block->used)
		lastchar = *(block->buff + block->used - 1);
	else if (block_count > 1)
		lastchar = *(block->prev->buff + BUFSIZE - 1);
	return lastchar == '\n';
}

Block *seekstart(Block *block, int nl_rqstd, int *start)
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
