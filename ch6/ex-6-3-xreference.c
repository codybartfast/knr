/*
 * Exercise 6-3.
 *
 * Write a cross-referencer that prints a list of all words in a document, and,
 * for each word, a list of the line numbers on which it occurs.  Remove noise
 * words like "the," "and," and so on.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getword.h"

#define MAXWORD 100

char *noisewords[] = { "a",	"about", "all",	 "an",	  "and",  "as",
		       "at",	"be",	 "but",	 "by",	  "do",	  "for",
		       "from",	"get",	 "go",	 "have",  "he",	  "her",
		       "his",	"i",	 "if",	 "in",	  "it",	  "me",
		       "my",	"not",	 "of",	 "on",	  "one",  "or",
		       "out",	"say",	 "she",	 "so",	  "that", "the",
		       "their", "there", "they", "this",  "to",	  "up",
		       "we",	"what",	 "when", "which", "who",  "will",
		       "with",	"would", "you" };

struct wnode {
	char *key;
	struct lnode *lines;
	struct wnode *left;
	struct wnode *right;
	struct wordinfo *wi;
};
struct wnode *addword(struct wnode *, char *key, struct wordinfo *wi);
struct wnode *walloc(void);
void freewnode(struct wnode *wnode);
void wprint(struct wnode *);
char *keyfrom(char *key, char *word);
int isnoiseword(char *word, int n);

struct lnode {
	int line;
	struct lnode *next;
};
struct lnode *addline(struct lnode *, int line);
struct lnode *lalloc(void);
void freelnode(struct lnode *lnode);
void lprint(struct lnode *, int first);
char *strdup(const char *s);

int main(void)
{
	char key[MAXWORD];
	struct wordinfo *wi;
	struct wnode *wnode = NULL;
	int nnoise = (sizeof noisewords / sizeof(char *));

	pseudoalphas = "'-";

	while ((wi = getwordinfo(&streamin, MAXWORD)) != NULL)
		if (!isnoiseword(keyfrom(key, wi->word), nnoise))
			if ((wnode = addword(wnode, key, wi)) == NULL)
				return 1;
	wprint(wnode);
	freewnode(wnode);
	return 0;
}

struct wnode *addword(struct wnode *p, char *key, struct wordinfo *wi)
{
	int cond;
	if (p == NULL) {
		if ((p = walloc()) == NULL)
			return NULL;
		p->wi = wi;
		p->key = (char *)strdup(keyfrom(key, wi->word));
		p->left = p->right = NULL;
		if ((p->lines = addline(NULL, wi->line)) == NULL)
			return NULL;
	} else if ((cond = strcmp(key, p->key)) == 0) {
		if ((p->lines = addline(p->lines, wi->line)) == NULL)
			return NULL;
	} else if (cond < 0) {
		if ((p->left = addword(p->left, key, wi)) == NULL)
			return NULL;
	} else {
		if ((p->right = addword(p->right, key, wi)) == NULL)
			return NULL;
	}
	return p;
}

struct wnode *walloc(void)
{
	return (struct wnode *)malloc(sizeof(struct wnode));
}

void freewnode(struct wnode *wnode)
{
	if (wnode == NULL)
		return;
	freewnode(wnode->left);
	freewnode(wnode->right);
	freewordinfo(wnode->wi);
	free(wnode->key);
	freelnode(wnode->lines);
	free(wnode);
}

void wprint(struct wnode *wnode)
{
	if (!(wnode == NULL)) {
		wprint(wnode->left);
		printf("%s  ", wnode->key);
		lprint(wnode->lines, 1);
		printf("\n");
		wprint(wnode->right);
	}
}

char *keyfrom(char *key, char *word)
{
	char *k = key;
	while ((*k++ = tolower(*word++)))
		;
	return key;
}

int isnoiseword(char *word, int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, noisewords[mid])) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return 1;
	}
	return 0;
}

struct lnode *addline(struct lnode *p, int line)
{
	if (p == NULL) {
		if ((p = lalloc()) == NULL)
			return NULL;
		p->line = line;
		p->next = NULL;
	} else if (p->line == line) {
		return p;
	} else {
		if ((p->next = addline(p->next, line)) == NULL)
			return NULL;
	}
	return p;
}

struct lnode *lalloc(void)
{
	return (struct lnode *)malloc(sizeof(struct lnode));
}

void freelnode(struct lnode *lnode)
{
	if (lnode == NULL)
		return;
	freelnode(lnode->next);
	free(lnode);
}

void lprint(struct lnode *p, int first)
{
	if (p != NULL) {
		first ? printf("%d", p->line) : printf(", %d", p->line);
		lprint(p->next, 0);
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
