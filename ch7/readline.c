/* Cannot be use concurrently! */

#include <stdio.h>
#include <stdlib.h>
#include "readline.h"

static int (*source)(void);

static long int len = 1;
static char *line = NULL;
static char *end;
static int goteof;

char *growline(char *pos);
static char *read(void);

char *(*read_lines(int (*get_char)(void)))(void)
{
	source = get_char;
	if (line == NULL) {
		line = (char *)malloc(len * sizeof(char));
		if (line == NULL) {
			printf("error: Insufficient Mememory (readline init)");
			return NULL;
		}
		end = line + len;
	}
	goteof = 0;
	return &read;
}

char *growline(char *pos)
{
	char *l, *n;
	int newlen = 2 * len;
	char *new = (char *)malloc(newlen * sizeof(char));
	if (new == NULL)
		return NULL;
	for (l = line, n = new; l < end;)
		*n++ = *l++;
	pos = new + (pos - line);
	line = new;
	len = newlen;
	end = line + len;
	return pos;
}

char *read(void)
{
	int c;
	char *p = line;

	if (goteof)
		return NULL;

	while ((c = (*source)()) != '\n' && c != EOF) {
		*p++ = c;
		if (p == end && (p = growline(p)) == NULL)
			return READLINE_ERROR;
	}
	*p = '\0';
	if (c == '\n')
		return line;
	goteof = 1;
	return (p == line) ? NULL : line;
}
