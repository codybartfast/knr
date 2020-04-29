#include <stdio.h>
#include "tabstops.h"

#define MAXARGS 1024
#define NOTSET -1

int custstops[MAXARGS], ncust;
int firstrepeating = NOTSET;
int repeatsize = 8;

int atoi(char *s);
int max(int, int);

int dist2stop(int col)
{
	int rdist, cdist, i;

	rdist = col < firstrepeating ?
			(firstrepeating - col) :
			repeatsize - ((col - firstrepeating) % repeatsize);

	for (i = 0; i < ncust; i++) {
		if (col < custstops[i]) {
			cdist = custstops[i] - col;
			return rdist < cdist ? rdist : cdist;
		}
	}
	return rdist;
}

int istabstop(int col)
{
	return col > 0 && (dist2stop(col - 1) == 1);
}

void parsestops(int argc, char *argv[])
{
	char *vstr, first;
	int val, i;
	int maxcust = 0;

	if (argc > MAXARGS) {
		printf("error: More than %d arguments!\n", MAXARGS);
		return;
	}
	ncust = 0;
	for (i = 1; i < argc; i++) {
		vstr = argv[i];
		first = *vstr;
		if (first == '-' || first == '+')
			vstr++;
		val = atoi(vstr);
		if (first == '-') {
			firstrepeating = val - 1;
		} else if (first == '+')
			repeatsize = val;
		else
			maxcust = max(maxcust, custstops[ncust++] = val - 1);
	}
	/* if no -m then start repeating from last custom stop */
	firstrepeating = firstrepeating == NOTSET ? maxcust : firstrepeating;
}

int atoi(char *s)
{
	int i, n = 0;

	for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
		n = 10 * n + (s[i] - '0');
	return n;
}

int max(int a, int b)
{
	return a >= b ? a : b;
}
