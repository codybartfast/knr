#include "stream.h"

int getword(struct stream *, char *, int);
struct wordinfo *getwordinfo(struct stream *, int);
void freewordinfo(struct wordinfo *wi);
int asalpha(char c);
int asalnum(char c);

char *pseudoalphas;

struct wordinfo {
	char *word;
	long line;
	long pos;
};