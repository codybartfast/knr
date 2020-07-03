#include "stream.h"

enum {NO_FILTER = 0, FILTER_CODE = 1};

int getword(int filtered, char *, int);
struct wordinfo *getwordinfo(int filtered, int);
void freewordinfo(struct wordinfo *wi);
int asalpha(char c);
int asalnum(char c);

char *pseudoalphas;

struct wordinfo {
	char *word;
	long line;
	long pos;
};