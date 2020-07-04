#include <stdlib.h>
#include <string.h>
#include "table.h"

#define HASHSIZE 101

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

struct nlist *delist(char *name, struct nlist *np);
struct nlist *nllookup(char *s);
unsigned hash(char *s);
char *strdup(const char *);

static struct nlist *hashtab[HASHSIZE];

void undef(char *name)
{
	if (name == NULL) {
		return;
	} else {
		unsigned hashval = hash(name);
		hashtab[hashval] = delist(name, hashtab[hashval]);
	}
}

struct nlist *delist(char *name, struct nlist *np)
{
	if (np == NULL)
		return NULL;
	if (strcmp(name, np->name) == 0) {
		struct nlist *next = np->next;
		free(np->name);
		free(np->defn);
		free(np);
		return next;
	}
	np->next = delist(name, np->next);
	return np;
}

char *lookup(char *s)
{
	struct nlist *nl = nllookup(s);
	return (nl == NULL) ? NULL : nl->defn;
}

struct nlist *nllookup(char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = nllookup(name)) == NULL) {
		np = (struct nlist *)malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else
		free((void *)np->defn);
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

unsigned hash(char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

char *strdup(const char *s)
{
	char *p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}
