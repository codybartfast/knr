/*
 * Exercise 8-3.
 *
 * Design and write _flushbuf, fflush and fclose.
 */

#include <fcntl.h>
#include <stdlib.h>
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
static int _fillbuf(FILE *);
static int _flushbuf(int, FILE *);

#define BUFSIZE 1024
#define PERMS 0666

int fflush(FILE *fp)
{
	int n;

	if (fp == NULL) {
		int i, rslt = 0;
		for (i = 0; i < OPEN_MAX; i++)
			if ((fp = &_iob[i])->flag & _WRITE)
				rslt |= fflush(fp);
		return rslt;
	}

	if (notreadyfor(fp, _WRITE))
		return EOF;
	if (fp->base == NULL)
		return 0;
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

int main(int argc, char *argv[])
{
	FILE *source, *temp;
	char c, *spath, *tpath;

	if (argc != 3) {
		write(2, &"error: Expect 3 args!\n", 22);
		return 1;
	}
	spath = argv[1];
	tpath = argv[2];

	if ((source = fopen(spath, "r")) == NULL) {
		write(2, &"error: failed to open source!\n", 30);
		return 1;
	}

	if ((temp = fopen(tpath, "w")) == NULL) {
		write(2, &"error: failed to open temp to write!\n", 37);
		return 1;
	}

	/* copy from source to temp */
	while ((c = getc(source)) != EOF)
		putc(c, temp);

	fclose(source);
	fclose(temp);

	if ((temp = fopen(tpath, "r")) == NULL) {
		write(2, &"error: failed to open temp to read!\n", 36);
		return 1;
	}

	/* copy from temp to stdout */
	while ((c = getc(temp)) != EOF)
		putchar(c);

	fflush(NULL);

	return 0;
}
