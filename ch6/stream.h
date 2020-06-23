#ifndef HDR_STREAM
#define HDR_STREAM

#include <stdlib.h>

#define MAXCHBUF (1 << 1)

struct stream *newstream(int (*)(void));
struct stream streamin;

struct stream {
	int (*getcharacter)(void);
	int *chbuf;
	int chbufidx;
	long offset;
	long line;
	long pos;
};

int getch(struct stream *stream);
void ungetch(struct stream *stream, int c);

#endif
