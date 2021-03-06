/*
 * Exercise 7-6.
 *
 * Write a program to compare two files, printing the first line where they
 * differ.
 */

#include <stdio.h>
#include <string.h>

#define MAXLINE (1 << 10)

int main(int argc, char *argv[])
{
	char line1[MAXLINE], line2[MAXLINE];
	FILE *file1 = fopen(argv[1], "r"), *file2 = fopen(argv[2], "r");
	char *rslt1, *rslt2;
	int num = 0;

	do {
		num++;
		rslt1 = fgets(line1, MAXLINE, file1);
		rslt2 = fgets(line2, MAXLINE, file2);
	} while (rslt1 && rslt2 && !strcmp(rslt1, rslt2));

	if (rslt1 != rslt2)
		printf("line: %d\n< %s---\n> %s", num,
		       rslt1 ? rslt1 : "<EOF>\n", rslt2 ? rslt2 : "<EOF>\n");
	return 0;
}
