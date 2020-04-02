/*
 * Exercise 2-9
 *
 * In a two's complement number system, x &= (x-1) deletes the rightmost
 * 1-bit in x.  Explain Why.  Use this observation to write a faster version
 * of bitcount. *
 */

#include <stdio.h>

void test(unsigned x, unsigned expected);
int bitcount(unsigned x);

int main(void)
{
	printf(
"===========================================================================\n"
"\n"
"Let's first consider the case where x contains no 1-bits, i.e., x=0.  In\n"
"this case x-1 = -1 and its two's complements representation is  a sequence\n"
" of ones. e.g., FFFFFFF for a 32b int.  This gives us 00000000 & 1111111\n"
"= 00000000 which is the original vale of x.  As x has no 1-bits this is the\n"
"desired result.\n"
"\n");
	printf(
"Now assume x contains at least one 1-bit.  We can represent x in binary as:\n"
"\n"
"        ?..?10..0\n"
"\n"
"  * ?..? is zero or more bits that can be zeros or ones.\n"
"  * 1 the rightmost 1-bit\n"
"  * 0..0 is zero or more 0-bits\n"
"\n"
"Then x-1 will be:\n"
"\n"
"        ?..?01..1\n"
"\n"
"x & (x - 1) will then be:\n"
"\n"
"        ?..?00..0\n"
"\n"
"because ?..? will be unchanged (because 0&0=0, 1&1=1) and in all other\n"
"positions we will have 0&1 or 1&0.\n"
"===========================================================================\n"
"\n");

	test(0, 0);
	test(1, 1);
	test(3, 2);
	test(28, 3);
	test(0xFFFFFFFF, 32);

	return 0;
}

void test(unsigned x, unsigned expected)
{
	unsigned actual = bitcount(x);

	if (actual == expected) {
		printf("ok: x:%x -> %d\n", x, actual);
	} else {
		printf("FAIL: x:%x, -> %d (expected %d)\n", x, actual,
		       expected);
	}
}

int bitcount(unsigned x)
{
	int count;

	for (count = 0; x != 0; x &= (x - 1))
		count++;
	return count;
}
