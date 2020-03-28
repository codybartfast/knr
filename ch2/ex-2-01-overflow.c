#include <stdio.h>

int main(void)
{
	unsigned char charv;
	unsigned short shortv;
	unsigned int intv;
	unsigned long longv;

	/* unsigned char */
	charv = 128;
	printf("unsigned char\n");
	printf("=============\n");
	printf("          val: %d\n", charv);
	printf("        2*val: %d\n", 2 * charv);
	printf("    val=2*val: %d\n\n", charv = charv + charv);

	/* unsigned short */
	shortv = 32768;
	printf("unsigned short\n");
	printf("==============\n");
	printf("          val: %d\n", shortv);
	printf("        2*val: %d\n", 2 * shortv);
	printf("    val=2*val: %d\n\n", shortv = shortv + shortv);

	/* unsigned int */
	intv = 2147483648;
	printf("unsigned int\n");
	printf("============\n");
	printf("          val: %u\n", intv);
	printf("        2*val: %u\n", 2 * intv);
	printf("    val=2*val: %u\n\n", intv = intv + intv);

	/* unsigned long */
	longv = 9223372036854775808ul;
	printf("unsigned long\n");
	printf("=============\n");
	printf("          val: %lu\n", longv);
	printf("        2*val: %lu\n", 2 * longv);
	printf("    val=2*val: %lu\n\n", longv = longv + longv);

	return 0;
}

/*

unsigned char
=============
          val: 128
        2*val: 256
    val=2*val: 0

unsigned short
==============
          val: 32768
        2*val: 65536
    val=2*val: 0

unsigned int
============
          val: 2147483648
        2*val: 0
    val=2*val: 0

unsigned long
=============
          val: 9223372036854775808
        2*val: 0
    val=2*val: 0

*/