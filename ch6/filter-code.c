#include <stdio.h>
#include <time.h>
#include "filter-code.h"

struct filterstate {
	int mode;
};

struct filterstate filterstate = { CODE };
static int fltbuff[MAXCHBUF];
int filtered(void);
int filter_code(struct stream *stream, struct filterstate *state);
struct charinfo *parse_code(struct stream *stream, struct filterstate *state);
struct charinfo *newci(int ch, int mode, long line);
struct stream filteredin = { &filtered, fltbuff, 0, 0, 0, 0 };

int filtered(void)
{
	return filter_code(&streamin, &filterstate);
}

/* For backward compatibility with 6-1 & 6-2 */
int filter_code(struct stream *stream, struct filterstate *state)
{
	int c;
	struct charinfo *ci = parse_code(stream, state);
	if (ci == NULL)
		return EOF;
	if (ci->mode == CODE) {
		c = ci->ch;
		freeci(ci);
		return c;
	} else {
		freeci(ci);
		return filter_code(stream, state);
	}
}

struct charinfo *getparsed(void)
{
	return parse_code(&streamin, &filterstate);
}

struct charinfo *parse_code(struct stream *stream, struct filterstate *state)
{
	int c;
	long line; /* needed? */
	static struct charinfo *temp, *stored = NULL;

	/* delay(1); */

	if (stored != NULL) {
		temp = stored;
		stored = NULL;
		return temp;
	}

	line = stream->line;
	if ((c = getch(stream)) == EOF)
		return newci(EOF, CODE, line);
	switch (state->mode) {
	case CODE:
		switch (c) {
		case '#':
			state->mode = PREPROC;
			return newci(c, state->mode, line);
		case '/':
			if ((c = getch(stream)) == EOF) {
				return NULL;
			} else if (c == '*') {
				state->mode = COMMENT;
				stored = newci(c, CODE, stream->line);
			} else {
				ungetch(stream, c);
			}
			return newci('/', CODE, line);
		case '"':
			state->mode = DOUBLE;
			return newci(c, CODE, line);
		case '\'':
			state->mode = SINGLE;
			return newci(c, CODE, line);
		default:
			return newci(c, CODE, line);
		}
	case PREPROC:
		/* Assumes preproc statements are single line */
		switch (c) {
		case '\n':
			state->mode = CODE;
			return newci(c, state->mode, line);
		default:
			return newci(c, state->mode, line);
		}
	case COMMENT:
		switch (c) {
		case '*':
			if ((c = getch(stream)) == EOF) {
				return NULL;
			} else if (c == '/') {
				state->mode = CODE;
				stored = newci(c, CODE, stream->line);
				return newci('*', CODE, line);
			} else {
				ungetch(stream, c);
				return newci('*', COMMENT, line);
			}
		default:
			return newci(c, state->mode, line);
		}

	case DOUBLE:
		switch (c) {
		case '\\':
			if ((c = getch(stream)) == EOF)
				return NULL;
			else {
				stored = newci(c, state->mode, stream->line);
			}
			return newci('\\', state->mode, line);
		case '"':
			state->mode = CODE;
			return newci(c, state->mode, line);
		default:
			return newci(c, state->mode, line);
		}
	case SINGLE:
		switch (c) {
		case '\'':
			state->mode = CODE;
			return newci(c, state->mode, line);
		default:
			return newci(c, state->mode, line);
		}
	default:
		printf("error: Unexpected mode %d", state->mode);
		return NULL;
	}
}

struct charinfo *newci(int ch, int mode, long line)
{
	struct charinfo *ci =
		(struct charinfo *)malloc(sizeof(struct charinfo));
	if (ci == NULL) {
		printf("error: Out of memory (newci)\n");
		return NULL;
	}
	ci->ch = ch;
	ci->mode = mode;
	ci->line = line;
	return ci;
}

void freeci(struct charinfo *ci)
{
	free(ci);
}

/* void delay(int number_of_seconds)
{
	clock_t end = clock() + (1000 * number_of_seconds);
	while (clock() < end)
		;
} */