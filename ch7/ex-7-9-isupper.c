/*
 * Exercise 7-9
 * 
 * Functions like isupper can be implemented to save space or to save time.  
 * Explore both possibilities.
 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* optimized for space */
static int sisupper(int c);

/* optimized for time */
static int tisupper(int c);
static int map[UCHAR_MAX];
static void tisupper_init(void);

int main(void)
{
	int i;
	short s = -32768;

	tisupper_init();

	printf("iscntrl:  %5d\n", iscntrl('\b'));
	printf("ispunct:  %5d\n", ispunct(','));
	printf("isalnum:  %5d\n", isalnum('A'));
	printf("isupper:  %5d\n", isupper('A'));
	printf("islower:  %5d\n", islower('a'));
	printf("isalpha:  %5d\n", isalpha('A'));
	printf("isdigit:  %5d\n", isdigit('1'));
	printf("isxdigit: %5d\n", isxdigit('A'));
	printf("isspace:  %5d\n", isspace(' '));
	printf("isprint:  %5d\n", isprint('A'));
	printf("isgraph:  %5d\n", isgraph('A'));
	printf("short:  %hd\n", s);

	for (i = 0; i < UCHAR_MAX; i++)
		if ((isupper(i) || sisupper(i) || tisupper(i)) &&
		    (!isupper(i) || !sisupper(i) || !tisupper(i)))
			printf("oops! %c/%d, %d %d %d\n", i, i, isupper(i),
			       sisupper(i), tisupper(i));
	return 0;
}

/* 
 * SPACE: No extra variables.
 * TIME:  One switch, up to 6 comparisons, upto 5 logic operations and
 *        numerous execution routes through the code.
 */

int sisupper(int c)
{
	switch ('A') {
	case 0x41: /* ASCII */
		return 'A' <= c && c <= 'Z';
	case 0xC1: /* EBCDIC */
		return (('A' <= c && c <= 'I') || ('J' <= c && c <= 'R') ||
			('S' <= c && c <= 'Z'));
	default:
		fprintf(stderr, "error: Can't guess encoding. 'A'=0x%X\n", 'A');
		exit(1);
	}
}

/*
 * TIME:  1 bit operation, 1 addition, 1 derefecence only 1 route through code.
 * SPACE: (UCHAR_MAX + 1) * word size, 2KB on 64bit machine.
 * 
 * I presume the work done by tissupper_init could be done by the compiler.  As
 * could the encoding check by sisupper.
 */

int tisupper(int c)
{
	return *(map + (unsigned char)c);
}

void tisupper_init(void)
{
	int i;

	for (i = 0; i < UCHAR_MAX; i++)
		map[i] = sisupper(i);
}
