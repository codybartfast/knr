/*
 * Exercise 8-4.
 *
 * The standard library function
 *
 * 	int fseek(FILE *fp, long offset, int origin)
 *
 * is identical to lseek except that fp is a file pointer instead of a file
 * descriptor and the return value is an int status, not a position.  Write
 * fseek.  Make sure that your fseek coordinates properly with the buffering
 * done for the other functions of the library.
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#undef NULL
#define NULL 0
#define EOF (-1)
#define OPEN_MAX 20

typedef struct _iobuf {
	int cnt;
	char *ptr;
	char *base;
	int flag;
	int fd;
	int bufsize;
} FILE;

enum flags { _READ = 01, _WRITE = 02, _UNBUF = 04, _EOF = 010, _ERR = 020 };

#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)
#define isopen(p) ((p)->flag & (_READ | _WRITE))
#define markclosed(p) ((p)->flag &= ~(_READ | _WRITE))
#define notreadyfor(p, rd_wr) (((p)->flag & (rd_wr | _EOF | _ERR)) != rd_wr)

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc(x, stdout)

FILE _iob[OPEN_MAX] = { { 0, (char *)0, (char *)0, _READ, 0, 0 },
			{ 0, (char *)0, (char *)0, _WRITE, 1, 0 },
			{ 0, (char *)0, (char *)0, _WRITE | _UNBUF, 2, 0 } };

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

FILE *fopen(char *name, char *mode);
int fflush(FILE *);
int fclose(FILE *);
int fseek(FILE *fp, long offset, int origin);
static int _fillbuf(FILE *);
static int _flushbuf(int, FILE *);

#define BUFSIZE 1024
#define PERMS 0666

int fseek(FILE *fp, long offset, int origin)
{
	if (fp == NULL || !isopen(fp))
		return EOF;
	if (fp->flag & _WRITE) {
		if (fflush(fp))
			return EOF;
	} else {
		offset -= (origin == SEEK_CUR) ? fp->cnt : 0L;
		fp->cnt = 0;
	}
	if (lseek(fp->fd, offset, origin) == -1) {
		fp->flag |= _ERR;
		return EOF;
	} else {
		fp->flag &= ~_EOF;
		return 0;
	}
}

int fflush(FILE *fp)
{
	int n;

	if (fp == NULL) {
		int i, rslt = 0;
		for (i = 0; i < OPEN_MAX; i++)
			if (isopen(fp = &_iob[i]))
				rslt |= fflush(fp);
		return rslt;
	}

	if (fp->base == NULL || fp->flag & _READ)
		return 0;
	if (notreadyfor(fp, _WRITE))
		return EOF;
	if ((n = fp->bufsize - fp->cnt))
		if (write(fp->fd, fp->base, n) != n) {
			fp->flag |= _ERR;
			return EOF;
		}
	fp->cnt = fp->bufsize;
	fp->ptr = fp->base;
	return 0;
}

int _flushbuf(int c, FILE *fp)
{
	fp->cnt = 0;
	if (notreadyfor(fp, _WRITE))
		return EOF;
	if (fp->base == NULL) {
		fp->bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
		if ((fp->base = (char *)malloc(fp->bufsize)) == NULL)
			return EOF;
		fp->ptr = fp->base;
		fp->cnt = fp->bufsize;
	} else if (fflush(fp))
		return EOF;
	*fp->ptr++ = c;
	fp->cnt--;
	return 0;
}

int fclose(FILE *fp)
{
	int rslt = 0;
	if (fp == NULL)
		return EOF;
	if (fp->flag & _WRITE)
		rslt = fflush(fp);
	free(fp->base);
	fp->base = NULL;
	if (fp->fd <= 2)
		return rslt;
	markclosed(fp);
	return rslt | close(fp->fd);
}

FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if (!isopen(fp))
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
	fp->bufsize = 0;
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

int _fillbuf(FILE *fp)
{
	if (notreadyfor(fp, _READ))
		return EOF;
	if (fp->base == NULL) {
		fp->bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
		if ((fp->base = (char *)malloc(fp->bufsize)) == NULL)
			return EOF;
	}
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, fp->bufsize);
	if (--fp->cnt < 0) {
		if (fp->cnt == -1)
			fp->flag |= _EOF;
		if (fp->cnt == -2)
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char)*fp->ptr++;
}

int main(void)
{
	FILE *temp;
	int i;
	char c, *s, tpath[] = "temp-8-4.txt";
	char draft[] = "My Ham is Green and I like to Eat it.\n";

	write(1, "Started with:  ", 15);
	write(1, draft, 38);

	/* Write draft text to temp file */
	if ((temp = fopen(tpath, "w")) == NULL) {
		write(2, "error: failed to open temp to write!\n", 37);
		return 1;
	}
	for (s = draft; *s; s++)
		putc(*s, temp);

	/* edit the temp file */
	fseek(temp, 3L, SEEK_SET);
	for (s = "Dog"; *s; s++)
		putc(*s, temp);

	fseek(temp, 4L, SEEK_CUR);
	for (s = "Beige"; *s; s++)
		putc(*s, temp);

	fseek(temp, -8L, SEEK_END);
	for (s = "Pat"; *s; s++)
		putc(*s, temp);

	fclose(temp);

	write(1, "Ended with:    ", 15);

	/* read temp file and write to stdout */
	if ((temp = fopen(tpath, "r")) == NULL) {
		write(2, "error: failed to open temp to read!\n", 36);
		return 1;
	}
	while ((c = getc(temp)) != EOF)
		putchar(c);
	fflush(stdout);


	/* fseek with read and SEEK_CUR */
	fseek(temp, 0, SEEK_SET);
	write(1, "Read SEEK_CUR: ", 15);
	for (i = 0; i < 22; i++)
		putchar(getc(temp));
	fseek(temp, 8L, SEEK_CUR);
	while ((c = getc(temp)) != EOF)
		putchar(c);

	fclose(stdout);
	fclose(temp);
	return 0;
}

/*

Started with:  My Ham is Green and I like to Eat it.
Ended with:    My Dog is Beige and I like to Pat it.
Read SEEK_CUR: My Dog is Beige and I Pat it.

*/