/*
 * Exercise 2-6
 *
 * Write a function setbits (x, p, n, y) that returns x with the n bits that
 * begin at position p set to the right most n bits of y, leaving the other
 * bits unchanged.
 */

#include <stdio.h>

void test(unsigned x, unsigned p, unsigned n, unsigned y, unsigned expected);
unsigned setbits(unsigned x, unsigned p, unsigned n, unsigned y);

int main(void)
{
	test(0x11111111u, 23, 8, 0xaaaaaaaaU, 0x11aa1111u);
	test(0x11111111u, 31, 32, 0xaaaaaaaaU, 0xaaaaaaaaU);
	test(9713u, 11, 4, 3500u, 11761u);
	test(93u, 4, 3, 211u, 81u);

	return 0;
}

void test(unsigned x, unsigned p, unsigned n, unsigned y, unsigned expected)
{
	unsigned actual = setbits(x, p, n, y);

	if (actual == expected) {
		printf("ok: x:%x, p:%x, n:%x, y:%x -> %x\n", x, p, n, y,
		       actual);
	} else {
		printf("FAIL: x:%x, p:%x, n:%x, y:%x -> %x (expected %x)\n", x,
		       p, n, y, actual, expected);
	}
}

/*
 * If n has its maximum value (e.g. 32) then "<< n" is undefined (Reference 
 * Manual A7.8 p206).  So to avoid "<< 32" I use "<< (n - 1) << 1".  (On my
 * machine "<< 32" is treated as "<< 0".)
 */
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

/*
 * Why p+1-n And Not p-n?
 * ======================
 *
 * Consider selecting the middle 4 bits of 8:
 *
 *          vvvv
 * bits:  01010101
 *
 *  pos:  76543210
 *
 * Here p = 5 and n = 4.
 *
 * There are two bits to the right of the selection so we will need to shift
 * left by two "<< 2".
 *
 * But because p is zero-based p - n would give a shift of just 1, (5 - 4),
 * whereas p + 1 - n gives the correct value of 2.
 *
 * (It would be a right shift if talking about getbits instead of setbits).
 */
