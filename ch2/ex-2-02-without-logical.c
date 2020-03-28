/*
 * Exercise 2.2
 *
 * Write a loop equivalent to the for loop above without using && or ||.
 *
 * For Loop:
 *
 * 	for (i = 0; i < lim - 1 && (c = getchar()) != '\n' && c != EOF; ++i)
 * 		s[i] = c;
 */

/*
 * The above for-loop can be replaced with:
 *
 * 	enum bool { NO, YES };
 *
 *	i = 0;
 *	loop = YES;
 *	while (loop) {
 *		if (i >= lim - 1) {
 *			loop = NO;
 *		} else if ((c = getchar()) == '\n') {
 *			loop = NO;
 *		} else if (c == EOF) {
 *			loop = NO;
 *		} else {
 *			s[i] = c;
 *			++i;
 *		}
 *	}
 *
 * I'm sure there's much neater solutions out there.
 */

#include <stdio.h>
#define MAXLINE 1000

enum bool { NO, YES };

int get_line(char line[], int maxline);

/* echo file putting ">> " at start of each line */
int main(void)
{
	int len;
	char line[MAXLINE];

	while ((len = get_line(line, MAXLINE)) > 0)
		printf(">> %s", line);
	return 0;
}

int get_line(char s[], int lim)
{
	int c, i, loop;

	i = 0;
	loop = YES;
	while (loop) {
		if (i >= lim - 1) {
			loop = NO;
		} else if ((c = getchar()) == '\n') {
			loop = NO;
		} else if (c == EOF) {
			loop = NO;
		} else {
			s[i] = c;
			++i;
		}
	}
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}
