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
struct charinfo *newci(int ch, int mode);
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
	static struct charinfo *temp, *stored = NULL;

	if (stored != NULL) {
		temp = stored;
		stored = NULL;
		return temp;
	}

	if ((c = getch(stream)) == EOF)
		return newci(EOF, CODE);
	switch (state->mode) {
	case CODE:
		switch (c) {
		case '#':
			state->mode = PREPROC;
			return newci(c, state->mode);
		case '/':
			if ((c = getch(stream)) == EOF) {
				return NULL;
			} else if (c == '*') {
				state->mode = COMMENT;
				stored = newci(c, CODE);
			} else {
				ungetch(stream, c);
			}
			return newci('/', CODE);
		case '"':
			state->mode = DOUBLE;
			return newci(c, CODE);
		case '\'':
			state->mode = SINGLE;
			return newci(c, CODE);
		default:
			return newci(c, CODE);
		}
	case PREPROC:
		/* Assumes preproc statements are single line */
		switch (c) {
		case '\n':
			state->mode = CODE;
			return newci(c, state->mode);
		default:
			return newci(c, state->mode);
		}
	case COMMENT:
		switch (c) {
		case '*':
			switch (c = getch(stream)) {
			case EOF:
				return NULL;
			case '/':
				state->mode = CODE;
				stored = newci(c, CODE);
				return newci('*', CODE);
			default:
				ungetch(stream, c);
				return newci('*', COMMENT);
			}
		default:
			return newci(c, state->mode);
		}

	case DOUBLE:
		switch (c) {
		case '\\':
			if ((c = getch(stream)) == EOF)
				return NULL;
			stored = newci(c, state->mode);
			return newci('\\', state->mode);
		case '"':
			state->mode = CODE;
			return newci(c, state->mode);
		default:
			return newci(c, state->mode);
		}
	case SINGLE:
		switch (c) {
		case '\'':
			state->mode = CODE;
			return newci(c, state->mode);
		default:
			return newci(c, state->mode);
		}
	default:
		printf("error: Unexpected mode %d", state->mode);
		return NULL;
	}
}

struct charinfo *newci(int ch, int mode)
{
	struct charinfo *ci =
		(struct charinfo *)malloc(sizeof(struct charinfo));
	if (ci == NULL) {
		printf("error: Out of memory (newci)\n");
		return NULL;
	}
	ci->ch = ch;
	ci->mode = mode;
	return ci;
}

void freeci(struct charinfo *ci)
{
	free(ci);
}
