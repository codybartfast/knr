/*
 * Exercise 8-3.
 *
 * Design and write _flushbuf, fflush and fclose.
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
	int flag;
	int fd;
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum flags { _READ = 01, _WRITE = 02, _UNBUF = 04, _EOF = 010, _ERR = 020 };

FILE *fopen(char *name, char *mode);
int _fillbuf(FILE *);
int _flushbuf(int, FILE *);
int fflush(FILE *);
int fclose(FILE *);

#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERR) != 0)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar() getc(stdin)
#define putchar(x) putc(x, stdout)

FILE _iob[OPEN_MAX] = { { 0, (char *)0, (char *)0, _READ, 0 },
			{ 0, (char *)0, (char *)0, _WRITE, 1 },
			{ 0, (char *)0, (char *)0, _WRITE | _UNBUF, 2 } };

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
		if ((fp->flag & (_READ | _WRITE)) == 0)
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
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

#include <stdlib.h>
#define BUFSIZE 1024

int _fillbuf(FILE *fp)
{
	int bufsize;

	if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
	if (fp->base == NULL)
		if ((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
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

int _flushbuf(int c, FILE *fp)
{
	if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
		return EOF;
	if (fp->base == NULL) {
		int bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
		if ((fp->base = (char *)malloc(bufsize)) == NULL)
			return EOF;
		fp->ptr = fp->base;
		fp->cnt = bufsize;
	} else if (fflush(fp))
		return EOF;
	*fp->ptr++ = c;
	fp->cnt--;
	return 0;
}

int fflush(FILE *fp)
{
	int bufsize;

	if (fp == NULL) {
		/* flush all open streams */
		int i, rslt = 0;
		for (i = 0; i < OPEN_MAX; i++)
			if ((fp = &_iob[i])->flag & (_READ | _WRITE))
				rslt |= fflush(fp);
		return rslt;
	}

	if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE)
		return EOF;
	if (fp->base == NULL)
		return 0;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
	if (write(fp->fd, fp->base, bufsize - fp->cnt) < 0) {
		fp->flag |= _ERR;
		return EOF;
	}
	fp->cnt = bufsize;
	fp->ptr = fp->base;
	return 0;
}

int fclose(FILE *fp)
{
	if (fp == NULL)
		return EOF;
	if (fp->flag & _WRITE)
		if (fflush(fp))
			return EOF;
	free(fp->base);
	fp->base = NULL;
	if (fp->fd <= 2)
		return 0;
	fp->flag &= ~(_READ | _WRITE); /* mark as unused */
	return close(fp->fd);
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
