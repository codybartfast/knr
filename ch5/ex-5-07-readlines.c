/*
 * Exercise 5-7.
 *
 * Rewrite readlines to store lines in an array supplied by main rather than
 * calling alloc to maintain storage.  How much faster is the program?
 */

#include <stdio.h>
#include <string.h>

#define MAXLINES 4000000
#define MAXLEN 100
#define BUFSIZE 8000000

int readlines(char *lineptr[], char *buffer);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);
void swap(char *v[], int i, int j);
int getline(char *, int);

char *lineptr[MAXLINES];
char buffer[BUFSIZE];

int main(void)
{
	int nlines;

	if ((nlines = readlines(lineptr, buffer)) >= 0) {
		qsort(lineptr, 0, nlines - 1); /* These lines removed for */
		writelines(lineptr, nlines);   /*   performance testing.  */
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}

int readlines(char *lineptr[], char *buffer)
{
	int len, nlines;
	char *bufmax = buffer + BUFSIZE - MAXLEN;

	nlines = 0;
	while ((len = getline(buffer, MAXLEN)) > 0) {
		lineptr[nlines++] = buffer;
		buffer += len;
		*(buffer - 1) = '\0';
		if (nlines >= MAXLINES || buffer > bufmax)
			return -1;
	}
	return nlines;
}

int getline(char s[], int lim)
{
	int c, i;
	for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

void writelines(char *lineptr[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

void swap(char *v[], int i, int j);

void qsort(char *v[], int left, int right)
{
	int i, last;

	if (left >= right)
		return;
	swap(v, left, (left + right) / 2);
	last = left;
	for (i = left + 1; i <= right; i++)
		if (strcmp(v[i], v[left]) < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	qsort(v, left, last - 1);
	qsort(v, last + 1, right);
}

void swap(char *v[], int i, int j)
{
	char *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}
