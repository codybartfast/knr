struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

struct nlist *install(char *name, char *defn);
char *lookup(char *s);
void undef(char *name);
