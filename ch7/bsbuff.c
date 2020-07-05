/*
 * Buffers the output from a provided source to apply any backspaces proveded by
 * that source.
 */

#include <stdio.h>
#include <stdlib.h>
#include "bsbuff.h"

#define SIZE (1 << 10)

static char buff[SIZE];
static char *last = buff + (SIZE - 1);

static int buffered(void);
static char read(void);

static int used;
static char *writep, *readp;
static int sourceexhausted;
static int (*source)(void);

int (*backspace_buffer(int (*get_char)(void)))(void)
{
	used = 0;
	readp = writep = buff;
	sourceexhausted = 0;
	source = get_char;
	return &buffered;
}

static int buffered(void)
{
	int c;

	if (sourceexhausted) {
		if (used == 0)
			return EOF;
		return read();
	}
	if (used == SIZE)
		return read();
	c = (*source)();
	if (c == EOF) {
		sourceexhausted = 1;
		return buffered();
	} else if (c == '\b') {
		if (used > 0) {
			writep = (writep == buff) ? last : writep - 1;
			used--;
		}
		return buffered();
	}
	*writep = c;
	writep = (writep == last) ? buff : writep + 1;
	used++;
	return buffered();
}

static char read(void)
{
	int c = *readp;

	readp = (readp == last) ? buff : readp + 1;
	used--;
	return c;
}
