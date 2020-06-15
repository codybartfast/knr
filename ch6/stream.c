#define STREAM_UNSET -3
#define STREAM_ERROR -2

#include <stdio.h>
#include "stream.h"

static int inbuff[MAXCHBUF];
struct stream streamin = { getchar, STREAM_UNSET, inbuff, 0 };

int getch(struct stream stream)
{
	stream.current = (stream.chbufidx > 0) ?
				 stream.chbuf[--stream.chbufidx] :
				 stream.read();
	return stream.current;
}

void ungetch(struct stream stream, int c)
{
	if (stream.chbufidx >= MAXCHBUF)
		printf("ungetch: too many characters\n");
	else {
		stream.chbuf[stream.chbufidx++] = c;
		stream.current = STREAM_UNSET;
	}
}
