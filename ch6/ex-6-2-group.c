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

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);

struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

int main(void)
{
	struct tnode *root;
	char word[MAXWORD];

	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (asalpha(word[0]))
			root = addtree(root, word);
	treeprint(root);
	return 0;
}

struct tnode *talloc(void);

struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0)
		p->count++;
	else if (cond < 0)
		p->left = addtree(p->left, w);
	else
		p->right = addtree(p->right, w);
	return p;
}

struct tnode *talloc(void)
{
	return (struct tnode *)malloc(sizeof(struct tnode));
}

void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
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
