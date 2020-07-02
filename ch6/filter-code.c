#include <stdio.h>
#include "filter-code.h"

enum filtermode { CODE = 0, PREPROC, COMMENT, DOUBLE, SINGLE };

struct filterstate filterstate = { CODE, 0 };
static int fltbuff[MAXCHBUF];
int filtered(void);
struct stream filteredin = { &filtered, fltbuff, 0, 0, 0, 0 };

int filtered(void)
{
	return filter_code(&streamin, filterstate);
}

int filter_code(struct stream *stream, struct filterstate state)
{
	int c = getch(stream);

	switch (state.mode) {
	case CODE:
		switch (c) {
		case '#':
			state.mode = PREPROC;
			return filter_code(stream, state);
		case '/':
			if ((c = getch(stream)) == '*') {
				state.mode = COMMENT;
				return filter_code(stream, state);
			} else {
				ungetch(stream, c);
				return '/';
			}
		case '"':
			state.mode = DOUBLE;
			return filter_code(stream, state);
		case '\'':
			state.mode = SINGLE;
			return filter_code(stream, state);
		default:
			return c;
		}
	case PREPROC:
		/* Assumes preproc statements are single line */
		switch (c) {
		case '\n':
			state.mode = CODE;
			return filter_code(stream, state);
		default:
			return state.incpreproc ? c :
						  filter_code(stream, state);
		}
	case COMMENT:
		switch (c) {
		case '*':
			if ((c = getch(stream)) == '/') {
				state.mode = CODE;
				return filter_code(stream, state);
			} else {
				ungetch(stream, c);
				return filter_code(stream, state);
			}
		default:
			return filter_code(stream, state);
		}

	case DOUBLE:
		switch (c) {
		case '\\':
			if (getch(stream) == EOF)
				return EOF;
			else { /* ignore escaped char */
			}
			return filter_code(stream, state);
		case '"':
			state.mode = CODE;
			return filter_code(stream, state);
		default:
			return filter_code(stream, state);
		}
	case SINGLE:
		switch (c) {
		case '\'':
			state.mode = CODE;
			return filter_code(stream, state);
		default:
			return filter_code(stream, state);
		}
	default:
		printf("error: Unexpected mode %d", state.mode);
		return EOF;
	}
}