#define STREAM_UNSET -2

#include <stdio.h>
#include "stream.h"

static int inbuff[MAXCHBUF];
struct stream streamin = { getchar, inbuff, 0 };

int getch(struct stream stream)
{
	return (stream.chbufidx > 0) ?
				 stream.chbuf[--stream.chbufidx] :
				 stream.read();
}

void ungetch(struct stream stream, int c)
{
	if (stream.chbufidx >= MAXCHBUF)
		printf("ungetch: too many characters\n");
	else {
		stream.chbuf[stream.chbufidx++] = c;
	}
}
