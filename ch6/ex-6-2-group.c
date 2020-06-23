/*
 * Exercise 6-2
 *
 * Write a program that reads a C program and prints in alphabetical order each
 * group of variable names that are identical in the first 6 characters, but
 * different somewhere thereafter.  Don't count words within strings and
 * comments.  Make 6 a parameter that can be set from the command line.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter-code.h"
#include "getword.h"

#define MAXWORD 100

char *reserved[] = { "auto",	 "break",   "case",   "char",	  "const",
		     "continue", "default", "do",     "double",	  "else",
		     "enum",	 "extern",  "float",  "for",	  "goto",
		     "if",	 "int",	    "long",   "register", "return",
		     "short",	 "signed",  "sizeof", "static",	  "struct",
		     "switch",	 "typedef", "union",  "unsigned", "void",
		     "volatile", "while" };

struct knode {
	char *key;
	struct wnode *wtree;
	struct knode *left;
	struct knode *right;
};
struct knode *addkey(struct knode *, char *k, char *w);
struct knode *kalloc(void);
void kprint(struct knode *);

struct wnode {
	char *word;
	struct wnode *left;
	struct wnode *right;
};
struct wnode *addword(struct wnode *, char *w);
struct wnode *walloc(void);
void wprint(struct wnode *);

int filtered(void);
struct filterstate filterstate;
int filterbuff[MAXCHBUF];
struct stream filteredstream = { filtered, filterbuff, 0 };

void parseargs(int argc, char *argv[]);
int isreserved(char *word, char *reserved[], int n);
char *getkey(char *word);

int nreserved;
char key[MAXWORD];
int keylen = 1;

int main(int argc, char *argv[])
{
	struct knode *ktree;
	char word[MAXWORD];

	nreserved = (sizeof reserved / sizeof(char *));

	parseargs(argc, argv);
	ktree = NULL;
	while (getword(filteredstream, word, MAXWORD) != EOF)
		if (asalpha(word[0]) &&
		    !(isreserved(word, reserved, nreserved))) {
			ktree = addkey(ktree, getkey(word), word);
			if (ktree == NULL) {
				printf("error: Out of memory!\n");
				return 1;
			}
		}
	kprint(ktree);
	return 0;
}

void parseargs(int argc, char *argv[])
{
	int len;

	if (!(argc == 2))
		return;
	if (len = atoi(*++argv))
		keylen = len;
}

int filtered(void)
{
	return filter_code(streamin, filterstate);
}

int isreserved(char *word, char *reserved[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, reserved[mid])) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return 1;
	}
	return 0;
}

char *getkey(char *word)
{
	int i;
	char *k = key;
	for (i = 0; i < keylen && *word; i++, k++, word++)
		*k = *word;
	*k = '\0';
	return key;
}

struct knode *addkey(struct knode *p, char *k, char *w)
{
	int cond;
	if (p == NULL) {
		if ((p = kalloc()) == NULL)
			return NULL;
		p->key = strdup(k);
		p->left = p->right = NULL;
		if ((p->wtree = addword(NULL, w)) == NULL)
			return NULL;
	} else if ((cond = strcmp(k, p->key)) == 0) {
		if ((p->wtree = addword(p->wtree, w)) == NULL)
			return NULL;
	} else if (cond < 0) {
		if ((p->left = addkey(p->left, k, w)) == NULL)
			return NULL;
	} else {
		if ((p->right = addkey(p->right, k, w)) == NULL)
			return NULL;
	}
	return p;
}

struct knode *kalloc(void)
{
	return (struct knode *)malloc(sizeof(struct knode));
}

void kprint(struct knode *p)
{
	if (p != NULL) {
		kprint(p->left);
		printf("%s:\n", p->key);
		wprint(p->wtree);
		kprint(p->right);
	}
}

struct wnode *addword(struct wnode *p, char *w)
{
	int cond;
	if (p == NULL) {
		if ((p = walloc()) == NULL)
			return NULL;
		p->word = strdup(w);
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0) {
		/* word already in tree */
	} else if (cond < 0) {
		if ((p->left = addword(p->left, w)) == NULL)
			return NULL;
	} else {
		if ((p->right = addword(p->right, w)) == NULL)
			return NULL;
	}
	return p;
}

struct wnode *walloc(void)
{
	return (struct wnode *)malloc(sizeof(struct wnode));
}

void wprint(struct wnode *p)
{
	if (p != NULL) {
		wprint(p->left);
		printf("    %s\n", p->word);
		wprint(p->right);
	}
}

char *strdup(const char *s)
{
	char *p;

	p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}
