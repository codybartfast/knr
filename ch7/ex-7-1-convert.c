/*
 * Exercise 7-1
 *
 * Write a program that converts upper case to lower or lower case to upper,
 * depending on the name it is invoked with, as found ing argv[0].
 */

/*
 * Converts to lower or upper depending on whether the name contains an 'l' or a
 * 'u' first.  If the name contains neither 'l' nor 'u' the input is unchanged.
 */

#include <ctype.h>
#include <stdio.h>

int (*getconvert(char *path))(int);
int identity(int);

int main(int argc, char *argv[])
{
	int c, (*convert)(int) = getconvert(argv[0]);

	(void)argc;
	while ((c = getchar()) != EOF)
		putchar((*convert)(c));
	return 0;
}

int (*getconvert(char *path))(int)
{
	char c, *file = path;

	while (*path)
		if (*path++ == '/')
			file = path;
	while ((c = tolower(*file++)))
		if (c == 'u')
			return &toupper;
		else if (c == 'l')
			return &tolower;
	return &identity;
}

int identity(int c)
{
	return c;
}
