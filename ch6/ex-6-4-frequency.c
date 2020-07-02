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
	char *key;
	int count;
	struct wnode *left;
	struct wnode *right;
};
struct wnode *addword(struct wnode *p, char *key, int *wc);
int treetoarray(struct wnode **warray, struct wnode *wnode, int i);
int compare(struct wnode *a, struct wnode *b);
struct wnode *walloc(void);
void freewnode(struct wnode *wnode);
char *keyfrom(char *key, char *word);
int isnoiseword(char *word);
char *strdup(const char *s);
int memerrcode(char *detail);

int main(void)
{
	char word[MAXWORD], key[MAXWORD];
	int i, wordcount = 0;
	struct wnode **warray, *wnode = NULL;

	pseudoalphas = "'-";

	while (getword(NO_FILTER, word, MAXWORD) != EOF)
		if (!isnoiseword(keyfrom(key, word)))
			if ((wnode = addword(wnode, key, &wordcount)) == NULL)
				return memerrcode("addword");

	warray = (struct wnode **)malloc(wordcount * sizeof(struct wnode *));
	if (warray == NULL)
		return memerrcode("warray");
	treetoarray(warray, wnode, 0);
	quicksort((void **)warray, 0, wordcount - 1, (VOIDCOMP)compare);
	for (i = 0; i < wordcount; i++)
		printf("%4d  %s\n", warray[i]->count, warray[i]->key);

	freewnode(wnode);
	free(warray);
	return 0;
}

struct wnode *addword(struct wnode *p, char *key, int *wc)
{
	int cond;
	if (p == NULL) {
		if ((p = walloc()) == NULL)
			NULL;
		p->key = (char *)strdup(key);
		p->left = p->right = NULL;
		p->count = 1;
		(*wc)++;
	} else if ((cond = strcmp(key, p->key)) == 0) {
		p->count++;
	} else if (cond < 0) {
		if ((p->left = addword(p->left, key, wc)) == NULL)
			return NULL;
	} else {
		if ((p->right = addword(p->right, key, wc)) == NULL)
			return NULL;
	}
	return p;
}

int treetoarray(struct wnode **warray, struct wnode *wnode, int idx)
{
	if (!wnode)
		return idx;
	warray[idx++] = wnode;
	idx = treetoarray(warray, wnode->left, idx);
	return treetoarray(warray, wnode->right, idx);
}

int compare(struct wnode *a, struct wnode *b)
{
	return b->count - a->count;
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
	free(wnode->key);
	free(wnode);
}

char *keyfrom(char *key, char *word)
{
	char *k = key;
	while ((*k++ = tolower(*word++)))
		;
	return key;
}

int isnoiseword(char *word)
{
	static char *noisewords[] = {
		"a",	 "about", "all",   "an",    "and",  "as",   "at",
		"be",	 "but",	  "by",	   "do",    "for",  "from", "get",
		"go",	 "have",  "he",	   "her",   "his",  "i",    "if",
		"in",	 "it",	  "me",	   "my",    "not",  "of",   "on",
		"one",	 "or",	  "out",   "say",   "she",  "so",   "that",
		"the",	 "their", "there", "they",  "this", "to",   "up",
		"we",	 "what",  "when",  "which", "who",  "will", "with",
		"would", "you"
	};
	static int n = (sizeof noisewords / sizeof(char *));

	int cond, low, high, mid;

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
	char *p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}

int memerrcode(char *detail)
{
	printf("error: out of memory. (%s)\n", detail);
	return 1;
}
