#include <stdio.h>
#include "detab.h"

static int tsize = 4;
static int (*source)(void);
static int maxlen;

static int column;
static int pending;

static int dist2stop(int col, int length);
static int replace(void);

void configure_tabs(int tabsize)
{
	tsize = tabsize;
}

int (*detab(int (*get_char)(void), int linelen))(void)
{
	source = get_char;
	maxlen = linelen;
	pending = column = 0;
	return &replace;
}

int nextstop(int col)
{
	return (((col + 1) / tsize) + 1) * tsize;
}

/* replace assumes result is always greater than zero */
int dist2stop(int col, int linelen)
{
	int ns;
	if (col >= linelen)
		return 1;
	if ((ns = nextstop(col)) > linelen)
		return (linelen - col);
	return ns - col;
}

int replace(void)
{
	int c;

	if (pending) {
		pending--;
		column++;
		return ' ';
	}
	switch (c = (*source)()) {
	case '\n':
		column = 0;
		return c;
	case '\t':
		pending = dist2stop(column, maxlen) - 1;
		column++;
		return ' ';
	default:
		column++;
		return c;
	}
}