/*
 * Exercise 6-4.
 *
 * Write a program that prints the distinct words in its input sorted into
 * decreasing order of frequency of occurrence.  Precede each word by its count.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "getword.h"
#include "quicksort.h"

#define MAXWORD 100

struct wnode {
	struct wordinfo *wi;
	char *key;
	int count;
	struct wnode *left;
	struct wnode *right;
};
struct wnode *addword(struct wnode *p, char *key, struct wordinfo *wi, int *wc);
int compare(struct wnode *a, struct wnode *b);
void wprint(struct wnode *);
struct wnode *walloc(void);
void freewnode(struct wnode *wnode);
int nodetoarray(struct wnode **warray, struct wnode *wnode, int i);
char *keyfrom(char *key, char *word);
char *strdup(const char *s);
int isnoiseword(char *word, int n);

char *noisewords[] = { "a",	"about", "all",	 "an",	  "and",  "as",
		       "at",	"be",	 "but",	 "by",	  "do",	  "for",
		       "from",	"get",	 "go",	 "have",  "he",	  "her",
		       "his",	"i",	 "if",	 "in",	  "it",	  "me",
		       "my",	"not",	 "of",	 "on",	  "one",  "or",
		       "out",	"say",	 "she",	 "so",	  "that", "the",
		       "their", "there", "they", "this",  "to",	  "up",
		       "we",	"what",	 "when", "which", "who",  "will",
		       "with",	"would", "you" };

int main(void)
{
	int i;

	struct wordinfo *wi;
	char key[MAXWORD];
	int nnoise = (sizeof noisewords / sizeof(char *));
	int wc = 0;
	struct wnode *wnode = NULL;
	struct wnode **warray;

	psudoalpha = '\'';

	while ((wi = getwordinfo(&streamin, MAXWORD)) != NULL)
		if (!isnoiseword(keyfrom(key, wi->word), nnoise))
			if ((wnode = addword(wnode, key, wi, &wc)) == NULL)
				return 1;

	warray = (struct wnode **)malloc(wc * sizeof(struct wnode *));
	nodetoarray(warray, wnode, 0);
	quicksort((void **)warray, 0, wc - 1, (VOIDCOMP)compare);
	for (i = 0; i < wc; i++)
		printf("%4d  %s\n", warray[i]->count, warray[i]->key);	
		
	freewnode(wnode);
	free(warray);
	return 0;
}

struct wnode *addword(struct wnode *p, char *key, struct wordinfo *wi, int *wc)
{
	int cond;
	if (p == NULL) {
		if ((p = walloc()) == NULL)
			return NULL;
		p->wi = wi;
		p->key = (char *)strdup(keyfrom(key, wi->word));
		p->left = p->right = NULL;
		p->count = 1;
		(*wc)++;
	} else if ((cond = strcmp(key, p->key)) == 0) {
		p->count++;
	} else if (cond < 0) {
		if ((p->left = addword(p->left, key, wi, wc)) == NULL)
			return NULL;
	} else {
		if ((p->right = addword(p->right, key, wi, wc)) == NULL)
			return NULL;
	}
	return p;
}

int compare(struct wnode *a, struct wnode *b)
{
	return b->count - a->count;
}

void wprint(struct wnode *wnode)
{
	if (!(wnode == NULL)) {
		wprint(wnode->left);
		printf("%4d  %s\n", wnode->count, wnode->key);
		wprint(wnode->right);
	}
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
	free(wnode);
}

int nodetoarray(struct wnode **warray, struct wnode *wnode, int i)
{
	warray[i++] = wnode;
	if (wnode->left)
		i = nodetoarray(warray, wnode->left, i);
	if (wnode->right)
		i = nodetoarray(warray, wnode->right, i);
	return i;
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

char *strdup(const char *s)
{
	char *p;

	p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}