#include "stream.h"

enum filtermode { CODE = 0, PREPROC, COMMENT, DOUBLE, SINGLE };

struct charinfo {
	int ch;
	enum filtermode mode;
};

struct charinfo *getparsed(void);
void freeci(struct charinfo *ci);
struct stream filteredin;