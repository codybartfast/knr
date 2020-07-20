/*
 * Exercise 8-1.
 * 
 * Rewrite the program cat from Chapter 7 using read, write, open and close 
 * instead of their standard library equivalents.  Perform experiments to 
 * determine the relative speeds of the two versions.
 */

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void filecopy(int ifd, char *ipath, int ofd, char *opath);
static void error(char *fmt, ...);

int main(int argc, char *argv[])
{
	int fd;
	char *path;

	if (argc == 1) {
		fd = 0;
		path = "<stdin>";
	} else if ((fd = open(path = argv[1], O_RDONLY, 0)) == -1) {
		error("%s: can't open %s\n", argv[0], path);
	}
	filecopy(fd, path, 1, "<stdout>");
	if (fd)
		close(fd);
	return 0;
}

void filecopy(int ifd, char *ipath, int ofd, char *opath)
{
	static char buf[BUFSIZ];
	int n;

	while ((n = read(ifd, buf, BUFSIZ)) > 0)
		if (write(ofd, buf, n) != n)
			error("Problem writing to '%s'", opath);
	if (n == -1)
		error("Problem reading from '%s'", ipath);
}

void error(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}

/*

        | Times sec (256MB)     | Avrg  |%linux |%syscal
--------|-----------------------|-------|-------|-------
lnxcat: | 9.590,  9.502,  9.721 | 9.604 |       |-22.7%
syscal: |12.378, 12.465, 12.421 |12.421 |+29.3% |
stdlib: |16.462, 16.524, 16.980 |16.655 |+73.4% |+34.1%

*/
