#include <stdio.h>
#include "stream.h"

static int inbuff[MAXCHBUF];
struct stream streamin = { getchar, inbuff, 0, 0, 0, 0 };

struct stream *newstream(int (*getcharacter)(void))
{
	struct stream *strm = (struct stream *)malloc(sizeof(struct stream));
	int *chbuf = (int *)malloc(MAXCHBUF * sizeof(int));
	if (strm == NULL || chbuf == NULL)
		return NULL;
	strm->getcharacter = getcharacter;
	strm->chbuf = chbuf;
	strm->chbufidx = 0;
	strm->offset = 0;
	strm->line = 0;
	strm->pos = 0;
	return strm;
}

int getch(struct stream *stream)
{
	int c;
	if (stream->chbufidx > 0) {
		return stream->chbuf[--stream->chbufidx];
	}
	c = stream->getcharacter();
	stream->offset++;
	if (c == '\n') {
		stream->line++;
		stream->pos = 0;
	} else if(c == '\t'){
		stream->pos = stream->pos + 8;
	} else {
		stream->pos++;
	}
	return c;
}

void ungetch(struct stream *stream, int c)
{
	if (stream->chbufidx >= MAXCHBUF)
		printf("ungetch: too many characters\n");
	else {
		stream->chbuf[stream->chbufidx++] = c;
	}
}
