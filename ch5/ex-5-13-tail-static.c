/*
 * Exercise 5-13
 *
 * Write the program tail, which prints the last n lines of its input.  By
 * default, n is 10, let us say, but it can be changed by an optional
 * argument, so that
 *
 * 	tail -n
 *
 * prints the last n lines.  The program should behave rationally no matter
 * how unreasonable the input or the value of n.  Write the program so it
 * makes the best use of available storage; lines should be stored as in the
 * sorting program of Section 5.6, not in a two-dimensional array of fixed
 * size.
 */

#include <stdio.h>

#define DEFAULTTAIL 10
#define MAXTAIL 1 << 20
#define BUFFSIZE 1 << 30

#define ADVANCEBUF (bufpt = bufpt++ < buflimit ? bufpt : buff)

int getline(char *buff, long buffsize, char *bufpt);
long atoi(char *s);
long min(long a, long b);

char *lines[MAXTAIL];
char buff[BUFFSIZE];

int main(int argc, char *argv[])
{
	long ntail, nlineptrs, nlnprint;
	char **line, **lnlimit, **nextline;
	long buffsize, len, nlninput;
	char *bufpt, *buflimit, c;

	ntail = (argc == 2 && *argv[1] == '-') ? atoi(argv[1] + 1) :
						 DEFAULTTAIL;
	ntail = min(ntail, MAXTAIL);
	line = lines;
	lnlimit = lines + ntail;

	buffsize = BUFFSIZE;
	bufpt = buff;
	buflimit = buff + buffsize;
	nlninput = 0;

	while ((len = getline(buff, buffsize, bufpt)) > 0) {
		*(line = line < lnlimit ? line : lines) = bufpt;
		bufpt = buff + ((bufpt + len - buff + 1) % buffsize);
		line++;
		nlninput++;
	}

	nlineptrs = min(nlninput, ntail);

	/* go back to oldest line, breaking if make full cycle of buffer */
	for (nlnprint = 0; nlnprint < nlineptrs; nlnprint++) {
		nextline = line--;
		line = line >= lines ? line : lines + ntail - 1;
		if (*line <= bufpt && bufpt <= *nextline) {
			line = nextline;
			break;
		}
	}

	/* go forward printing lines */
	while (nlnprint-- > 0) {
		bufpt = *line;
		while (*bufpt) 
			putchar(*ADVANCEBUF);
		line = ++line < lnlimit ? line : lines;
	}
	return 0;
}

/* read chars into buffer, circling to start when reaching end */
int getline(char *buff, long bufsize, char *bufpt)
{
	int c;
	char *buflimit;
	long len;

	len = 0;
	buflimit = buff + bufsize;
	while (len < bufsize - 1 && (c = getchar()) != EOF && c != '\n') {
		len++;
		*bufpt = c;
		ADVANCEBUF;
	}
	if (c == '\n') {
		len++;
		*bufpt = c;
		ADVANCEBUF;
	}
	*bufpt = '\0';
	if (len > 0) 
		ADVANCEBUF;
	return len;
}

long atoi(char *s)
{
	int i;
	long n = 0;

	for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
		n = 10 * n + (s[i] - '0');
	return n;
}

long min(long a, long b)
{
	return a <= b ? a : b;
}
