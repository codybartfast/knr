#include <ctype.h>
#include "stdio.h"
#include "align.h"
#include "readline.h"
#include "detab.h"

static int shift(void);
static int justify(void);
static char *trim(char *line);
static char *stats(char *line);

int _linelen, _maxroll, _align;
char *(*readline)(void);

static int width, tabs, spaces;

int (*align(int (*get_char)(void), int linelen, int maxroll, int align))(void)
{
	_linelen = linelen;
	_maxroll = maxroll;
	_align = align;

	readline = read_lines(get_char);

	return (align == ALIGN_JUSTIFIED) ? &justify : &shift;
}

int shift(void)
{
	static char *line = NULL, *lp;
	static int pad;

	if (line == NULL) {
		if ((line = readline()) == NULL || line == READLINE_ERROR)
			return EOF;

		lp = stats(trim(line));
		if (tabs || _align == ALIGN_LEFT)
			pad = 0;
		else if (_align == ALIGN_RIGHT)
			pad = _linelen - width;
		else if (_align == ALIGN_CENTRE)
			pad = ((_linelen - width) / 2);
		else {
			printf("error: unexpected alignment (%d)", _align);
			return EOF;
		}
	}

	if (pad) {
		pad--;
		return ' ';
	}

	if (*lp == '\0') {
		line = NULL;
		return '\n';
	}
	return *lp++;
}

int justify(void)
{
	char c;
	static char *line = NULL, *lp;
	static double rate, target = 0.5;
	static int actual = 0, pad = 0;

	if (line == NULL) {
		int tail;

		if ((line = readline()) == NULL || line == READLINE_ERROR)
			return EOF;
		lp = stats(trim(line));
		tail = _linelen - width;
		rate = (spaces && tail <= _maxroll) ?
			       ((double)(spaces + tail)) / spaces :
			       1;
	}

	if (pad) {
		pad--;
		return ' ';
	}

	if ((c = *lp++) == ' ') {
		target += rate;
		pad = target - actual - 1;
		actual += (pad + 1);
		return ' ';
	}

	if (c == '\0') {
		line = NULL;
		return '\n';
	}
	return c;
}

char *trim(char *line)
{
	char *lp;
	for (lp = line; *lp; lp++)
		;
	while (lp > line && isspace(*(lp - 1)))
		*--lp = '\0';

	return line;
}

char *stats(char *line)
{
	char c, *lp = line;

	width = tabs = spaces = 0;

	while ((c = *lp++) != '\0') {
		switch (c) {
		case '\t':
			width = nextstop(width);
			tabs++;
			break;
		case ' ':
			spaces++;
			/* fallthrough */
		default:
			width++;
		}
	}
	return line;
}