#include "stream.h"

struct filterstate {
	int mode;
	int incpreproc;
};

struct filterstate filterstate;
struct filterstate newfilterstate(void);
int filter_code(struct stream *stream, struct filterstate state);
struct stream filteredin;