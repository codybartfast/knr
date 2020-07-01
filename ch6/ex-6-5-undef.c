/*
 * Write a function undef that will remove a name and definition from the table
 * maintained by lookup and install.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

#define SPEED "speed"
#define COLOUR "colour"
#define ANIMAL "animal"

struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);
unsigned hash(char *s);
char *strdup(char *);

static struct nlist *hashtab[HASHSIZE];

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

int main(void)
{
	install(SPEED, "quick");
	install(COLOUR, "brown");
	install(ANIMAL, "cow");
	install(ANIMAL, "fox");

	printf("The %s, %s %s jumped over the lazy dog.%s\n",
	       lookup(SPEED)->defn, lookup(COLOUR)->defn, lookup(ANIMAL)->defn,
	       (char *)NULL);

	return 0;
}

struct nlist *lookup(char *s)
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

	if ((np = lookup(name)) == NULL) {
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
	return 0;
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

char *strdup(char *s)
{
	char *p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}
