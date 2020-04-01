/*
 * Exercise 2-7
 *
 * Write a function invert(x, p, n) that returns x with the n bits that
 * begin at position p inverted (i.e., 1 changed into 0 and vice versa),
 * leaving the others unchanged.
 */

#include <stdio.h>

void test(unsigned x, unsigned p, unsigned n, unsigned expected);
unsigned invert(unsigned x, unsigned p, unsigned n);
unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y);

int main(void)
{
	test(0x11111111u, 23, 8, 0x11ee1111u);
	test(0x456789ABu, 31, 32, 0xBA987654u);

	return 0;
}

void test(unsigned x, unsigned p, unsigned n, unsigned expected)
{
	unsigned actual = invert(x, p, n);

	if (actual == expected) {
		printf("ok: x:%x, p:%x, n:%x -> %x\n", x, p, n, actual);
	} else {
		printf("FAIL: x:%x, p:%x, n:%x, -> %x (expected %x)\n", x, p, n,
		       actual, expected);
	}
}

unsigned invert(unsigned x, unsigned p, unsigned n)
{
	return setbits(x, p, n, ~x);
}

unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y)
{
	unsigned mask;
	if (n == 0) {
		mask = 0;
	} else {
		mask = ~(~0 << (n - 1) << 1) << (p + 1 - n);
	}
	return (x & ~mask) | (y & mask);
}