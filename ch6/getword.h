#include "stream.h"

int getword(struct stream *, char *, int);
struct wordinfo *getwordinfo(struct stream *, int);
int asalpha(char c);
int asalnum(char c);

char psudoalpha;

struct wordinfo {
	char *word;
	long line;
	long pos;
};