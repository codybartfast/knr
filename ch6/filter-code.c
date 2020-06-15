#include <stdio.h>
#include "filter-code.h"

enum filtermode { CODE = 0, PREPROC, COMMENT, DOUBLE, SINGLE };

struct filterstate newfilterstate(void)
{
	struct filterstate s = { CODE };
	return s;
}

int filter_code(struct stream stream, struct filterstate state)
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
		switch (c) {
		case '\n':
			state.mode = CODE;
			return filter_code(stream, state);
		default:
			return filter_code(stream, state);
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
			/* ignore escaped char */
			return filter_code(stream, state);
		case '"':
			state.mode = CODE;
			return filter_code(stream, state);
		default:
			return filter_code(stream, state);
		}
	case SINGLE:
		switch (c) {
		case '\\':
			if (getch(stream) == EOF)
				return EOF;
			/* ignore escaped char */
			return filter_code(stream, state);
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