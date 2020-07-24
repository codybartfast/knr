/*
 * Exercise 8-8
 * 
 * Write a routine bfree(p, n) that will free an arbitrary block p of n 
 * characters into the free list maintained by malloc and free.  By usning 
 * bfree, a user can add a static or external array to the free list at any 
 * time.
 */

#include <stddef.h>
#include <stdlib.h>

typedef long Align;

union header {
	struct {
		union header *ptr;
		unsigned size;
	} s;
	Align x;
};

void bfree(void *, unsigned);

void bfree(void *p, unsigned n)
{
	union header *hp;
	int hsize = sizeof(union header);
	int units = n / hsize;
	if (units >= 2) {
		(hp = (union header *)p)->s.size = units;
		free(hp + 1);
	}
}
