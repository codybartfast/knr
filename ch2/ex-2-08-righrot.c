/*
 * Exercise 2-8
 *
 * Write a function rightrot(x, n) that returns the value of the integer x
 * rotated to the right n bit positions.
 */

#include <stdio.h>

void test(unsigned x, unsigned n, unsigned expected);
unsigned rightrot(unsigned x, unsigned n);
int sizeof_uint(void);

int main(void)
{
	test(0x123456u, 0, 0x123456u);
	test(0x123456u, 4, 0x60012345u);
	test(0x123456u, 16, 0x34560012u);
	test(0x123456u, 28, 0x1234560u);
	test(0x123456u, 32, 0x123456u);

	return 0;
}

void test(unsigned x, unsigned n, unsigned expected)
{
	unsigned actual = rightrot(x, n);

	if (actual == expected) {
		printf("ok: x:%x, n:%d -> %x\n", x, n, actual);
	} else {
		printf("FAIL: x:%x, n:%d, -> %x (expected %x)\n", x, n, actual,
		       expected);
	}
}

unsigned rightrot(unsigned x, unsigned n)
{
	unsigned bits = sizeof_uint();

	if (n == 0 || n == bits)
		return x;

	return (x >> n) | x << (bits - n);
}

/* size of uint in bits */
int sizeof_uint(void)
{
	unsigned x2, x = 1;
	int bits = 1;

	while (x < (x2 = x * 2)) {
		x = x2;
		++bits;
	}
	return bits;
}

/*
 * SAMPLES
 * =======
 * x:123456, n:0 -> 123456
 * x:123456, n:4 -> 60012345
 * x:123456, n:16 -> 34560012
 * x:123456, n:28 -> 1234560
 * x:123456, n:32 -> 123456
 */
