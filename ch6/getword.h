#include "stream.h"

int getword(struct stream *, char *, int);
struct wordinfo *getwordinfo(struct stream *, char *, int);
int asalpha(char c);
int asalnum(char c);

struct wordinfo {
	char *word;
	long line;
	long pos;
};