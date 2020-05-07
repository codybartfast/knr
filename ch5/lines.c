#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "lines.h"

#define INITIAL_BUFFSIZE (1 << 1)
#define INITIAL_LINESSIZE (1 << 0)

unsigned long maxlen = ULONG_MAX;

int new_buff(char **buffref, char **bufflimitref, unsigned long length);
int enlarge_buff(char **buffref, char **cursorref, char **bufflimitref,
		 char **lines, char **curline);
int new_lines(char ***linesref, char ***lineslimit, unsigned long length);
int enlarge_lines(char ***linesref, char ***currlnref, char ***lineslimitref);

int readlines(char **buffref, char ***linesref)
{
	char *buff;
	char *cursor;
	char *bufflimit;

	char **lines;
	char **currln;
	char **lineslimit;

	char c, prevc = '\0';

	if (new_buff(&buff, &bufflimit, INITIAL_BUFFSIZE) == LNS_ERROR)
		return LNS_ERROR;

	if (new_lines(&lines, &lineslimit, INITIAL_LINESSIZE) == LNS_ERROR)
		return LNS_ERROR;

	*lines = cursor = buff;
	currln = lines;

	while ((c = getchar()) != EOF) {
		if (currln >= lineslimit) {
			if (enlarge_lines(&lines, &currln, &lineslimit) ==
			    LNS_ERROR)
				return LNS_ERROR;
		}
		if (cursor >= bufflimit - 1) {
			if (enlarge_buff(&buff, &cursor, &bufflimit, lines,
					 currln) == LNS_ERROR)
				return LNS_ERROR;
		}
		if (prevc == '\n') {
			*cursor++ = '\0';
			*++currln = cursor;
		}
		*cursor++ = prevc = c;
	}
	*buffref = buff;
	*linesref = lines;
	return 1 + currln - lines;
}

int new_buff(char **buffref, char **bufflimit, unsigned long length)
{
	*buffref = malloc(length * sizeof(char));
	if (*buffref == NULL) {
		return LNS_ERROR;
		printf("error: Insufficient memory (allocating buff)\n");
	}
	*bufflimit = *buffref + length;
	return LNS_OK;
}

int enlarge_buff(char **buffref, char **cursorref, char **bufflimitref,
		 char **lines, char **curline)
{
	char *old = *buffref;
	char *oldptr = old;
	char *oldlimit = *bufflimitref;
	unsigned long oldlen = oldlimit - old;

	char *new, *newptr, *newlimit;
	unsigned long newlen;

	char **lptr;

	if (oldlen == maxlen) {
		printf("error: Cannot increase length beyond %ld\n", oldlen);
		return LNS_ERROR;
	}

	newlen = oldlen > maxlen / 4 ? maxlen : oldlen * 4;
	if (new_buff(&new, &newlimit, newlen) == LNS_ERROR)
		return LNS_ERROR;
	newptr = new;

	while (oldptr < oldlimit)
		*newptr++ = *oldptr++;

	for (lptr = lines; lptr <= curline; lptr++)
		*lptr = new + (*lptr - old);

	*buffref = new;
	*bufflimitref = newlimit;
	*cursorref = new + (*cursorref - old);
	free(old);
	return LNS_OK;
}

int new_lines(char ***linesref, char ***lineslimit, unsigned long length)
{
	*linesref = malloc(length * sizeof(char *));
	if (*linesref == NULL) {
		return LNS_ERROR;
		printf("error: Insufficient memory (allocating lines)\n");
	}
	*lineslimit = *linesref + length;
	return LNS_OK;
}

int enlarge_lines(char ***linesref, char ***currlnref, char ***lineslimitref)
{
	char **old = *linesref;
	char **oldptr = old;
	char **oldlimit = *lineslimitref;
	unsigned long oldlen = oldlimit - old;

	char **new, **newptr, **newlimit;
	unsigned long newlen;

	if (oldlen == maxlen) {
		printf("error: Cannot increase length beyond %ld\n", oldlen);
		return LNS_ERROR;
	}

	newlen = oldlen > maxlen / 4 ? maxlen : oldlen * 4;
	if (new_lines(&new, &newlimit, newlen) == LNS_ERROR)
		return LNS_ERROR;
	newptr = new;

	while (oldptr <= oldlimit)
		*newptr++ = *oldptr++;

	*linesref = new;
	*lineslimitref = newlimit;
	*currlnref = new + (*currlnref - old);
	free(old);
	return LNS_OK;
}

void writelines(char **lines, int nlines)
{
	char **line, **linelimit = lines + nlines;

	for (line = lines; line < linelimit; line++)
		printf("%s", *line);
}

void freelines(char *buff, char **lines){
	free(lines);
	free(buff);
}
