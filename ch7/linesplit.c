
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "linesplit.h"
#include "detab.h"

#define UNSET -1

static int maxlen;
static int maxroll = 8;
static int (*source)(void);

static int buffsize;
static char *buff = NULL;
static int lineend;
static int next;
static int column;
static int pending;
static int puthyphen = 0;
static int goteof = 0;

static int split(void);
static void store(char c);

int (*split_lines(int (*get_char)(void), int length))(void)
{
	maxlen = length;
	source = get_char;

	buffsize = maxlen + 1;
	free(buff);
	buff = (char *)malloc(buffsize * sizeof(char));
	if (buff == NULL) {
		printf("error: No memory (linesplit buff)\n");
		return NULL;
	}
	column = next = pending = 0;
	lineend = UNSET;

	return &split;
}

int split(void)
{
	int i, c;

	/* all done */
	if (goteof && (lineend == UNSET))
		return EOF;

	/* return line content from buff */
	if (pending < lineend) {
		c = *(buff + pending++);
		return c;
	} else if (lineend >= 0) {
		/* have returned buffered content, now finish this line */
		if (puthyphen) {
			puthyphen = 0;
			return '-';
		}
		/* copy unused chars to start of (next) line */
		int end = next;
		column = next = 0;
		for (; pending < end; pending++)
			store(*(buff + pending));

		pending = 0;
		lineend = UNSET;
		return '\n';
	}

	/* read chars into buff */
	c = (*source)();
	if (c == EOF) {
		goteof = 1;
		lineend = next;
	} else if (column <= maxlen && c == '\n') {
		lineend = next;
	} else if (column < maxlen) {
		store(c);
	} else if (isspace(c)) {
		/* ignore a space immediately after end of this line */
		lineend = next;
	} else {
		/* store first char that doesn't fit in this line */
		store(c);
		/* go back looking for recent space or hyphen */
		for (i = next - 2; i > next - 2 - maxroll; i--) {
			c = *(buff + i);
			if (isspace(c) || c == '-') {
				lineend = i + 1;
				break;
			}
		}
		/* if needed, split in the middle of the chars we've checked */
		if (lineend == UNSET) {
			puthyphen = 1;
			lineend = next - 1 - (maxroll / 2);
		}
	}
	return split();
}

void store(char c)
{
	*(buff + next++) = c;
	column = (c == '\t') ? nextstop(column) : column + 1;
}
