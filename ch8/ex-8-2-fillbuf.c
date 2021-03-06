/*
 * Exercise 8-2.
 *
 * Rewrite fopen and _fillbuf with fields instead of explicit bit operations.
 * Compare code size and execution speed.
 */

/**********
 *  FILE  *
 **********/

#define NULL 0
#define EOF (-1)
#define OPEN_MAX 20

typedef struct _iobuf {
	int cnt;
	char *ptr;
	char *base;
	int read;
	int write;
	int unbuf;
	int eof;
	int err;
	int fd;
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

FILE *fopen(char *name, char *mode);
int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) write(p->fd, &x, 1);

#define getchar() getc(stdin)
#define putchar(x) putc(x, stdout)

/***********
 *  fopen  *
 ***********/

#include <fcntl.h>
#include <unistd.h>
#define PERMS 0666

FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if (!fp->read && !fp->write)
			break;
	if (fp >= _iob + OPEN_MAX)
		return NULL;
	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else
		fd = open(name, O_RDONLY, 0);
	if (fd == -1)
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	fp->read = (*mode == 'r');
	fp->write = !fp->read;
	fp->unbuf = fp->eof = fp->err = 0;
	return fp;
}

/**************
 *  _fillbuf  *
 **************/

#include <stdlib.h>
#define BUFSIZE 1024

int _fillbuf(FILE *fp)
{
	int bufsize;

	if (!fp->read || fp->eof || fp->err)
		return EOF;
	bufsize = fp->unbuf ? 1 : BUFSIZE;
	if (fp->base == NULL)
		if ((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->eof = 1;
		if (fp->cnt == -2)
			fp->err = 1;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char)*fp->ptr++;
}

/**************************
 *  stdin, stdou, stderr  *
 **************************/

FILE _iob[OPEN_MAX] = { { 0, (char *)0, (char *)0, 1, 0, 0, 0, 0, 0 },
			{ 0, (char *)0, (char *)0, 0, 1, 0, 0, 0, 1 },
			{ 0, (char *)0, (char *)0, 0, 1, 1, 0, 0, 2 } };

/****************
 *  main (cat)  *
 ****************/

int main(int argc, char *argv[])
{
	FILE *fp;
	char c;

	while (--argc > 0)
		if ((fp = fopen(*++argv, "r")) == NULL)
			return 1;
		else
			while ((c = getc(fp)) != EOF)
				putchar(c);
	return 0;
}

/*

Either of them is at least 100 times (10,000%) slower than linux cat.

TIME    | cat times (s/2MB)     | Avrg  | %
--------|-----------------------|-------|-------
bits:   |21.773, 22.342, 22.393 |22.169 |
fields: |23.242, 23.218, 25.834 |16.655 | +8.7%

Fields resulted in an absolute increase of 1.8% in the size of the binary.
If you look at size of the programs over a 'minimal' program it is 26.5% bigger.

SIZE    | Abs   | %     | Extra | %
--------|-------|-------|-------|-------
minimal:|16,472 |       |       |
bits:   |17,648 |       |+1,176 |
fields: |17,960 | +1.8% |+1,488 |+26.5%

*/
