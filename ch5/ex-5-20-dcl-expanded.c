/* Exercise 5-20.
 *
 * Expand dcl to handle declarations with function argument types, qualifiers
 * like const, and so on.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSYMBL (1 << 7)
#define MAXMSG (1 << 10)
#define MAXCHBUF (1 << 1)
#define MAXTKBUF (1 << 1)

enum { STORE, QUAL, TYPE, VAR, BRACKETS };
enum { OK = 0, ERROR };
enum { NO = 0, YES };

int declaration(char *dec, int isdef);
int dcl(char *name, char *out, int allowanon);
int dirdcl(char *name, char *out, int allowanon);
int args(char *out);

int gettoken(void);
void ungettoken(int ttype, char *tk);
int ws(void);
int name(char *p);
int brackets(char *p);
int oparens(char *p);
int contains(char **names, int count, char *name);

void nextdef(void);
int getch(void);
void ungetch(int c);

int tokentype;
char token[MAXSYMBL];

int chbuf[MAXCHBUF];
int chbufp = 0;

int ttbuf[MAXTKBUF];
char tkbuf[MAXTKBUF][MAXSYMBL];
int tkbufp = 0;

char *types[] = { "void",  "char",   "short",  "int",	  "long",
		  "float", "double", "signed", "unsigned" };
int ntypes = 9;
char *stores[] = { "auto", "register", "static", "extern" };
int nstores = 4;
char *quals[] = { "const", "volatile" };
int nquals = 2;

int main(void)
{
	char dec[MAXMSG];
	while (gettoken() != EOF) {
		if (declaration(dec, YES) != OK) {
			nextdef();
		}
	}
	return 0;
}

int declaration(char *dec, int isdef)
{
	char store[MAXSYMBL + 12];
	char qual[MAXSYMBL + 12];
	char type[MAXMSG];
	char name[MAXSYMBL];
	char out[MAXMSG];

	store[0] = '\0';
	qual[0] = '\0';
	type[0] = '\0';

	if (tokentype == STORE) {
		sprintf(store, " in %s storage", token);
		gettoken();
	}
	if (tokentype == QUAL) {
		sprintf(qual, " %s", token);
		gettoken();
	}
	if (tokentype != TYPE) {
		printf("\nerror: Expected a type\n");
		return ERROR;
	}
	do {
		if (type[0] != '\0')
			strcat(type, " ");
		strcat(type, token);
	} while (gettoken() == TYPE);
	ungettoken(tokentype, token);

	do {
		out[0] = '\0';
		if (dcl(name, out, !isdef) != OK) {
			return ERROR;
		} else if (tokentype != ';' && tokentype != ',' &&
			   tokentype != ')') {
			printf("\nsytax error, got %d/%c\n", tokentype,
			       tokentype);
			return ERROR;
		} else {
			sprintf(dec, "%s:%s%s %s%s", name, qual, out, type,
				store);
			if (isdef)
				printf("\n%s\n", dec);
		}
	} while (isdef && tokentype == ',');
	return OK;
}

int dcl(char *name, char *out, int allowanon)
{
	int ns, rslt;

	for (ns = 0; gettoken() == '*';)
		ns++;
	if ((rslt = dirdcl(name, out, allowanon)) != OK)
		return rslt;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return OK;
}

int dirdcl(char *name, char *out, int allowanon)
{
	int rslt;

	if (tokentype == VAR)
		strcpy(name, token);
	else if (tokentype == '(') {
		if ((rslt = dcl(name, out, allowanon)) != OK)
			return rslt;
		if (tokentype != ')') {
			printf("\nerror: missing )\n");
			return ERROR;
		}
	} else if(allowanon){
		name[0] = '\0';
		allowanon = NO;
		ungettoken(tokentype, token);
	} else {
		printf("\nerror: expected variable name or (dcl)\n");
		return ERROR;
	}
	while (gettoken() == '(' || tokentype == BRACKETS) {
		if (tokentype == '(') {
			strcat(out, " function taking");
			if ((rslt = args(out)) != NO) {
				return rslt;
			}
		} else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
	}
	return OK;
}

int args(char *out)
{
	char dec[MAXMSG];
	int argcount = 0;
	char *seperator = " argument ";
	int expectarg = YES;

	if (gettoken() == ')') {
		/* fun() */
		expectarg = NO;
	} else if (tokentype == TYPE && strcmp(token, "void") == 0) {
		if (gettoken() == ')')
			/* fun(void) */
			expectarg = NO;
		else {
			ungettoken(tokentype, token);
			tokentype = TYPE;
			strcpy(token, "void");
		}
	}

	if (expectarg) {
		do {
			if (argcount++ > 0)
				gettoken();
			if (declaration(dec, NO) != OK)
				return ERROR;
			strcat(out, seperator);
			strcat(out, dec);
			seperator = " and argument ";
		} while (tokentype == ',');
	}
	if (tokentype == ')') {
		if (argcount == 0)
			strcat(out, " no arguments");
		strcat(out, " returning");
	} else {
		printf("\nerror: expected closing parentheses "
		       "after arguments (got %d/%c)\n",
		       tokentype, tokentype);
		return ERROR;
	}
	return OK;
}

int gettoken(void)
{
	if (tkbufp > 0) {
		--tkbufp;
		tokentype = ttbuf[tkbufp];
		strcpy(token, tkbuf[tkbufp]);
	} else {
		ws();
		if (!(oparens(token) || brackets(token) || name(token))) {
			token[0] = tokentype = getch();
			token[1] = '\0';
		}
	}
	return tokentype;
}

void ungettoken(int ttype, char *tk)
{
	if (tkbufp >= MAXTKBUF) {
		printf("ungettoken: too many tokens\n");
	} else {
		ttbuf[tkbufp] = ttype;
		strcpy(tkbuf[tkbufp], tk);
		tkbufp++;
	}
}

int ws(void)
{
	char c;
	int rslt = NO;

	while (isspace(c = getch()))
		rslt = YES;
	ungetch(c);
	return rslt;
}

int name(char *p)
{
	char c, *tkn;
	int rslt = NO;

	tkn = p;
	if (isalpha(c = getch()) || c == '_') {
		rslt = YES;
		for (*p++ = c; isalnum(c = getch()) || c == '_';)
			*p++ = c;
		*p = '\0';
		if (contains(types, ntypes, tkn))
			tokentype = TYPE;
		else if (contains(stores, nstores, tkn))
			tokentype = STORE;
		else if (contains(quals, nquals, tkn))
			tokentype = QUAL;
		else
			tokentype = VAR;
	}
	ungetch(c);
	return rslt;
}

int brackets(char *p)
{
	char c;

	if ((c = getch()) == '[') {
		for (*p++ = c; (*p++ = (c = getch())) != ']';)
			;
		*p = '\0';
		tokentype = BRACKETS;
		return YES;
	}
	ungetch(c);
	return NO;
}

int oparens(char *p)
{
	char c;

	if ((c = getch()) == '(') {
		*p++ = tokentype = '(';
		*p = '\0';
		return YES;
	}
	ungetch(c);
	return NO;
}

void nextdef(void)
{
	int c;

	while ((c = getch()) != ';' && c != EOF)
		;
	if (c == EOF)
		ungetch(c);
}

int contains(char **names, int count, char *name)
{
	int i;

	for (i = 0; i < count; i++)
		if (strcmp(name, names[i]) == 0)
			return YES;
	return NO;
}

int getch(void)
{
	return (chbufp > 0) ? chbuf[--chbufp] : getchar();
}

void ungetch(int c)
{
	if (chbufp >= MAXCHBUF)
		printf("ungetch: too many characters\n");
	else
		chbuf[chbufp++] = c;
}

/*
 * input
 *

int simple;

int dec, *larator, lists[];

static char *storage;

volatile int qualifier;

long unsigned int compound;

void arguments(char *name, double time);

int nested_args(char *(*read)(void), void (*write)(char *message));

static const long unsigned int (*book2)[13], *book3[13], complex(
	volatile char (*(*book6(void))[])(
		char **book1,
		void *book4(),
		void (*book5)()
	),
	char (*(*book7[3])())[5]
);

 *
 * output
 *

simple: int

dec: int

larator: pointer to int

lists: array[] of int

storage: pointer to char in static storage

qualifier: volatile int

compound: long unsigned int

arguments: function taking argument name: pointer to char and argument time:
double returning void

nested_args: function taking argument read: pointer to function taking no
arguments returning pointer to char and argument write: pointer to function
taking argument message: pointer to char returning void returning int

book2: const pointer to array[13] of long unsigned int in static storage

book3: const array[13] of pointer to long unsigned int in static storage

complex: const function taking argument
	book6: volatile function taking no arguments returning pointer to
	array[] of pointer to function taking argument
		book1: pointer to pointer to char
	and argument
		book4: function taking no arguments returning pointer to void
	and argument
		book5: pointer to function taking no arguments returning void
	returning char
and argument
	book7: array[3] of pointer to function taking no arguments returning
	pointer to array[5] of char
returning long unsigned int in static storage

 *
 */