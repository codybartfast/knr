/* Exercise 8-6.
 * 
 * The standard library function calloc(n, size) returns a pointer to n objects
 * of size size, with the storage initialized to zero.  Write calloc, by calling
 * malloc or by modifying.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *Calloc(unsigned n, unsigned size);

int main(void)
{
	char source[] = "Hello I am the source string.\n";
	char *target = Calloc(strlen(source) + 1, sizeof(char));
	strcpy(target, source);
	printf(target);
	return 0;
}

void *Calloc(unsigned n, unsigned size)
{
	size_t len = n * size;
	char *b, *p;

	if ((b = p = malloc(len)) == NULL)
		return NULL;
	while (--len)
		*p++ = 0;
	return b;
}
