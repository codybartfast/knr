/*
 * Exercise 2-1
 *
 * Write a program to determine the ranges of char, short, int, and long
 * variables, both signed an unsigned, by printing appropriate values from
 * standard headers and by direct computation.  Harder if you compute them:
 * determine the ranges of the various floating-point types.
*/

#include <float.h>
#include <limits.h>
#include <stdio.h>

int sizeof_uchar(void);
int sizeof_ushrt(void);
int sizeof_uint(void);
int sizeof_ulong(void);

unsigned long power(unsigned long base, unsigned long n);
unsigned long power_less_one(unsigned long base, unsigned long n);

int main(void)
{
	int charbits = sizeof_uchar();
	int shrtbits = sizeof_ushrt();
	int intbits = sizeof_uint();
	int longbits = sizeof_ulong();

	printf("\n");
	printf("Char\n");
	printf("====\n");
	printf("Bits:              %4d\n", CHAR_BIT);
	printf("Max:               %4d\n", CHAR_MAX);
	printf("Min:               %4d\n", CHAR_MIN);
	printf("Signed Max:        %4d\n", SCHAR_MAX);
	printf("Signed Max calc:   %4ld\n", power_less_one(2, charbits - 1));
	printf("Signed Min:        %4d\n", SCHAR_MIN);
	printf("Signed Min calc:   %4ld\n", -power(2, charbits - 1));
	printf("Unsigned Max:      %4d\n", UCHAR_MAX);
	printf("Unsigned Max calc: %4ld\n", power_less_one(2, charbits));

	printf("\n");
	printf("Short\n");
	printf("=====\n");
	printf("Max:               %6d\n", SHRT_MAX);
	printf("Max calc:          %6ld\n", power_less_one(2, shrtbits - 1));
	printf("Min:               %6d\n", SHRT_MIN);
	printf("Min calc:          %6ld\n", -power(2, shrtbits - 1));
	printf("Unsigned Max:      %6u\n", USHRT_MAX);
	printf("Unsigned Max calc: %6lu\n", power_less_one(2, shrtbits));

	printf("\n");
	printf("Int\n");
	printf("===\n");
	printf("Max:               %11d\n", INT_MAX);
	printf("Max calc:          %11ld\n", power_less_one(2, intbits - 1));
	printf("Min:               %11d\n", INT_MIN);
	printf("Min calc:          %11ld\n", -power(2, intbits - 1));
	printf("Unsigned Max:      %11u\n", UINT_MAX);
	printf("Unsigned Max calc: %11lu\n", power_less_one(2, intbits));

	printf("\n");
	printf("Long\n");
	printf("====\n");
	printf("Max:               %20ld\n", LONG_MAX);
	printf("Max calc:          %20ld\n", power_less_one(2, longbits - 1));
	printf("Min:               %20ld\n", LONG_MIN);
	printf("Min calc:          %20ld\n", -power(2, longbits - 1));
	printf("Unsigned Max:      %20lu\n", ULONG_MAX);
	printf("Unsigned Max calc: %20lu\n", power_less_one(2, longbits));

	printf("\n");
	printf("Float\n");
	printf("=====\n");
	printf("Radix:    %39d\n", FLT_RADIX);
	printf("Rounds:   %5d\n", FLT_ROUNDS);
	printf("Digits:   %5d\n", FLT_DIG);
	printf("Epsilon:  %7.1f\n", FLT_EPSILON);
	printf("Mantissa: %5d\n", FLT_MANT_DIG);
	printf("Max:      %5.1f\n", FLT_MAX);
	printf("Max Exp:  %5d\n", FLT_MAX_EXP);
	printf("Min:      %7.1f\n", FLT_MIN);
	printf("Min Exp:  %5d\n", FLT_MIN_EXP);

	printf("\n");
	printf("Double\n");
	printf("======\n");
	printf("Digits:   %5d\n", DBL_DIG);
	printf("Epsilon:  %7.1f\n", DBL_EPSILON);
	printf("Mantissa: %5d\n", DBL_MANT_DIG);
	printf("Max:      %f\n", DBL_MAX);
	printf("Max Exp:  %5d\n", DBL_MAX_EXP);
	printf("Min:      %7.1f\n", DBL_MIN);
	printf("Min Exp:  %5d\n", DBL_MIN_EXP);

	return 0;
}

int sizeof_uchar(void)
{
	unsigned char dbl, val = 1;
	int size = 1;

	while (val < (dbl = val * 2)) {
		val = dbl;
		++size;
	}
	return size;
}

/* int sizeof_ushrt(void)
{
	unsigned short val = 1;
	int size = 1;

	while (val < val * 2) {
		val = val * 2;
		++size;
	}
	return size;
} */

int sizeof_ushrt(void)
{
	unsigned short dbl, val = 1;
	int size = 1;

	while (val < (dbl = val * 2)) {
		val = dbl;
		++size;
	}
	return size;
}

int sizeof_uint(void)
{
	unsigned int dbl, val = 1;
	int size = 1;

	while (val < (dbl = val * 2)) {
		val = dbl;
		++size;
	}
	return size;
}

int sizeof_ulong(void)
{
	unsigned long dbl, val = 1;
	int size = 1;

	while (val < (dbl = val * 2)) {
		val = dbl;
		++size;
	}
	return size;
}

unsigned long power(unsigned long base, unsigned long n)
{
	unsigned long i, p;

	p = 1;
	for (i = 0; i < n; ++i)
		p = p * base;
	return p;
}

unsigned long power_less_one(unsigned long base, unsigned long n)
{
	return (power(base, n - 1UL) * (base - 1UL)) +
	       (power(base, n - 1UL) - 1UL);
}