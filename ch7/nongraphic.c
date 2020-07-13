/*
 * Given a source returns a new source that has any non-graphic ascii chars
 * replaced with their hex value, e.g., DEL -> <0x7f>.
 */

#include <limits.h>
#include <stdio.h>
#include "nongraphic.h"

#define FORMAT "<0x%02x>"
#define LENGTH 6

static char replacement[LENGTH + 1];
static char *pending, *end;

static int (*_get_char)(void);

static int replace(void);

int (*replace_nongraphic(int (*get_char)(void)))(void)
{
	_get_char = get_char;

	if (UCHAR_MAX > 0xff) {
		printf("error: UCHAR_MAX larger than 0Xff: %ud", UCHAR_MAX);
		return NULL;
	}
	pending = end = replacement + LENGTH;
	return &replace;
}

static int replace(void)
{
	if (pending < end) {
		return *pending++;
	} else {
		int c = (*_get_char)();
		if ((0x20 <= c && c <= 0x7e) || (0x08 <= c && c <= 0x0C) ||
		    c == EOF)
			return c;
		sprintf(replacement, FORMAT, c);
		pending = replacement;
		return replace();
	}
}
