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
#include "getword.h"

#define MAXWORD 100

char *getkey(char *word);

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

char key[MAXWORD];
int keylen = 1;

int main(void)
{
	struct knode *ktree;
	char word[MAXWORD];

	ktree = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (asalpha(word[0]))
			ktree = addkey(ktree, getkey(word), word);
	kprint(ktree);
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
		p = kalloc();
		p->key = strdup(k);
		p->left = p->right = NULL;
		p->wtree = addword(NULL, w);
	} else if ((cond = strcmp(k, p->key)) == 0) {
		p->wtree = addword(p->wtree, w);
	} else if (cond < 0)
		p->left = addkey(p->left, k, w);
	else
		p->right = addkey(p->right, k, w);
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
		p = walloc();
		p->word = strdup(w);
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0) {
	} else if (cond < 0)
		p->left = addword(p->left, w);
	else
		p->right = addword(p->right, w);
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
