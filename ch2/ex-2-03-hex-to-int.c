/*
 * Exercise 2-3
 *
 * Write the function htoi(s), which converts a string of hexadecimal digits
 * (including an optinal 0x or 0X) into its equivalent integer value.  The
 * allowable digits are 0 through 9, a through f, and A through F.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define NONHEX -1

const unsigned long ERROR = -2;

void test(char str[], long expected);
long htoi(char str[]);
int hctoi(char c);
int error(char msg[], char str[]);

int main(void)
{
	test("", ERROR);
	test("G", ERROR);
	test("0X", ERROR);
	test("0xZ", ERROR);
	test("0", 0);
	test("0x5", 5);
	test("0X9", 9);
	test("A", 10);
	test("0xC", 12);
	test("0XF", 15);
	test("5G stop w/o error with non-hex", 5);
	test("F123", 61731);
	test("0xABC", 2748);
	test("0xFFFFFFFFFFFFFFFF", -1);
	return 0;
}

void test(char str[], long expected)
{
	long act = htoi(str);

	if (act == expected) {
		printf("ok: %s -> %ld\n", str, act);
	} else {
		printf("FAIL: \"%s\" -> %ld (expected %ld)\n", str, act,
		       expected);
	}
}

long htoi(char str[])
{
	int len = strlen(str);
	int idx = 0;
	int cv;
	long rslt;

	if (len >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
		idx = idx + 2;
	if (idx == len)
		return error("No characters to convert", str);

	if ((rslt = hctoi(str[idx])) == NONHEX)
		return error("No hex characters found", str);

	for (++idx; (cv = hctoi(str[idx])) != NONHEX; ++idx)
		rslt = rslt * 16 + cv;

	return rslt;
}

/* hex char to int */
int hctoi(char c)
{
	if (isdigit(c))
		return c - '0';

	/* A..F is contiguous even with EBCDIC */
	if ('A' <= (c = toupper(c)) && c <= 'F')
		return 10 + c - 'A';

	return NONHEX;
}

int error(char msg[], char str[])
{
	printf("ERROR: %s (str: %s)\n", msg, str);
	return ERROR;
}
