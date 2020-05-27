#define MAXCHBUF (1 << 1)

#include <stdio.h>
#include "getch.h"

int chbuf[MAXCHBUF];
int chbufp = 0;

int getch(void)
{
	return (chbufp > 0) ? chbuf[--chbufp] : getchar();
}

void ungetch(int c)
{
	if (chbufp >= MAXCHBUF)
		printf("ungetch: too many characters\n");
	else
		chbuf[chbufp++] = c;
}
