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

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

void undef(char *name);
struct nlist *delist(char *name, struct nlist *np);
char *defn(struct nlist *np);
void settab(void);
void display(void);
struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);
unsigned hash(char *s);
char *strdup(const char *);

static struct nlist *hashtab[HASHSIZE];

int main(void)
{
	settab();
	display();

	settab();
	undef(SPEED);
	display();

	settab();
	undef(COLOUR);
	display();

	settab();
	undef(ANIMAL);
	display();

	return 0;
}

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

char *defn(struct nlist *np)
{
	return np ? np->defn : "????";
}

void settab(void)
{
	int i;
	for (i = 0; i < HASHSIZE; i++)
		hashtab[i] = NULL;
	install(SPEED, "quick");
	install(COLOUR, "brown");
	install(ANIMAL, "fox");
}

void display(void)
{
	printf("The %s, %s %s jumped over the lazy dog.\n", defn(lookup(SPEED)),
	       defn(lookup(COLOUR)), defn(lookup(ANIMAL)));
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
	/* unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE; */
}

char *strdup(const char *s)
{
	char *p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}
