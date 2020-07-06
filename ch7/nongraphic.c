/*
 * Given a source returns a new source that has any non-graphic ascii chars
 * replaced with their hex value, e.g., DEL -> <0x7f>.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "nongraphic.h"

#define FORMAT "<0x%02x>"
#define LENGTH 6

static char replacement[LENGTH + 1];
static char *end;
static char *pending;

static int replace(void);

static int (*source)(void);

int (*replace_nongraphic(int (*get_char)(void)))(void)
{
	if (UCHAR_MAX > 0xff) {
		printf("error: UCHAR_MAX larger than 0Xff: %ud", UCHAR_MAX);
		return NULL;
	}
	end = replacement + LENGTH;
	pending = end;

	source = get_char;
	return &replace;
}

static int replace(void)
{
	if (pending < end) {
		return *pending++;
	} else {
		int c = (*source)();
		if ((0x20 <= c && c <= 0x7e) || (0x8 <= c && c <= 0xC) ||
		    c == EOF)
			return c;
		sprintf(replacement, FORMAT, c);
		pending = replacement;
		return replace();
	}
}
