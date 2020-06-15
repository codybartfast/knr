#define MAXCHBUF (1 << 1)

struct stream streamin;

struct stream {
	int (*read)(void);
	int *chbuf;
	int chbufidx;
};

int getch(struct stream stream);
void ungetch(struct stream stream, int c);
