/*
 * Exercise 7-7.
 * 
 * Modify the pattern finding program of Chapter 5 to take its input from a set
 * of named files or, if no files are named as arguments, from the standard 
 * input.  Should the file name be printed when a matching line is found?
 */

#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int get_line(char *line, int max);

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	long lineno = 0;
	int c, except = 0, number = 0, found = 0;

	while (--argc > 0 && (*++argv)[0] == '-') {
		while ((c = *++argv[0])) {
			switch (c) {
			case 'x':
				except = 1;
				break;
			case 'n':
				number = 1;
				break;
			default:
				printf("find illegal option %c\n", c);
				argc = 0;
				found = -1;
				break;
			}
		}
	}
	if (argc != 1) {
		printf("Usage: find -x -n pattern\n");
	} else {
		while (get_line(line, MAXLINE) > 0) {
			lineno++;
			if ((strstr(line, *argv) != NULL) != except) {
				if (number)
					printf("%ld:", lineno);
				printf("%s", line);
				found++;
			}
		}
	}
	return found;
}

int get_line(char s[], int lim)
{
	int c, i;
	for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i)
		if (i < lim - 1)
			s[i] = c;
	if (c == '\n') {
		if (i < lim - 1)
			s[i] = c;
		++i;
	}
	if (i < lim)
		s[i] = '\0';
	else
		s[lim - 1] = '\0';
	return i;
}