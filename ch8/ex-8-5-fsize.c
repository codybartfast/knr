/* Exercise 8-5.
 *
 * Modify the fsize program to print the other information contained in the 
 * inode entry.
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void fsize(char *);
void dirwalk(char *, void (*fcn)(char *));

int main(int argc, char *argv[])
{
	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			fsize(*++argv);
	return 0;
}

void fsize(char *name)
{
	struct stat stbuf;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & __S_IFMT) == __S_IFDIR)
		dirwalk(name, fsize);
	printf("%8ld %s\n", stbuf.st_size, name);
}

void dirwalk(char *dir, void (*fcn)(char *))
{
	char name[FILENAME_MAX];
	struct dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;
		if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalkd: name %s/%s too long\n", dir,
				dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}
