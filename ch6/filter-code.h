#include "stream.h"

struct filterstate {
	int mode;
};

struct filterstate newfilterstate(void);
int filter_code(struct stream stream, struct filterstate state);