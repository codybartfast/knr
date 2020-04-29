#include <stdio.h>
#include "tabstops.h"

#define MAXSTOPS 1024

int stops[MAXSTOPS], stopcount;
int dfltsize = 4;

int atoi(char *s);

int dist2stop(int col)
{
	int i;

	for (i = 0; i < stopcount; i++) {
		if (col < stops[i])
			return stops[i] - col;
	}
	return (dfltsize - (col % dfltsize));
}

int istabstop(int col)
{
	return col > 0 && (dist2stop(col - 1) == 1);
}

void parsestops(int argc, char *argv[])
{
	int i;

	stopcount = argc - 1;
	if (stopcount > MAXSTOPS) {
		printf("error: More than %d tap stops!\n", MAXSTOPS);
		return;
	}
	for (i = 1; i < argc; i++)
		stops[i - 1] = atoi(argv[i]) - 1;
}

int atoi(char *s)
{
	int i, n = 0;

	for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
		n = 10 * n + (s[i] - '0');
	return n;
}
