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
#define MAXBUF (1 << 1)
#define MAXTKBUF (1 << 1)

int declaration(char *dec, int isdef);
int dcl(char *name, char *out);
int dirdcl(char *name, char *out);
int params(char *out);

int gettoken(void);
void ungettoken(void);
int ws(void);
int name(char *p);
int brackets(char *p);
int oparens(char *p);
int contains(char **names, int count, char *name);

void nextline(void);
int getch(void);
void ungetch(int c);

enum { STORE, QUAL, TYPE, VAR, BRACKETS };
enum { OK = 0, ERROR };
enum { NO = 0, YES };

int tokentype;
char token[MAXSYMBL];

int buf[MAXBUF];
int bufp = 0;

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
			nextline();
		}
	}
	return 0;
}

int declaration(char *dec, int isdef)
{
	char store[MAXSYMBL];
	char qual[MAXSYMBL];
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
	ungettoken();

	do {
		out[0] = '\0';
		if (dcl(name, out) != OK) {
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

int dcl(char *name, char *out)
{
	int ns, rslt;

	for (ns = 0; gettoken() == '*';)
		ns++;
	if ((rslt = dirdcl(name, out)) != OK)
		return rslt;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return OK;
}

int dirdcl(char *name, char *out)
{
	int rslt;

	if (tokentype == VAR)
		strcpy(name, token);
	else if (tokentype == '(') {
		if ((rslt = dcl(name, out)) != OK)
			return rslt;
		if (tokentype != ')') {
			printf("\nerror: missing )\n");
			return ERROR;
		}
	} else {
		printf("\nerror: expected variable name or (dcl)\n");
		return ERROR;
	}
	while (gettoken() == '(' || tokentype == BRACKETS) {
		if (tokentype == '(') {
			strcat(out, " function taking");
			if ((rslt = params(out)) != NO) {
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

int params(char *out)
{
	char dec[MAXMSG];
	int argcount = 0;
	char *seperator = " argument ";
	int haveargs = YES;

	if (gettoken() == ')') {
		/* fun() */
		haveargs = NO;
	} else {
		if (tokentype == TYPE && strcmp(token, "void") == 0) {
			if (gettoken() == ')')
				/* fun(void) */
				haveargs = NO;
			else {
				ungettoken();
				tokentype = TYPE;
				strcpy(token, "void");
			}
		}
	}

	if (haveargs) {
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
		if (!haveargs)
			strcat(out, " no arguments");
		strcat(out, ", that returns");
	} else {
		printf("\nerror: expected closing parentheses "
		       "after paramaters (got %d/%c)\n",
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

void ungettoken(void)
{
	if (tkbufp >= MAXTKBUF) {
		printf("ungettoken: too many tokens\n");
	} else {
		ttbuf[tkbufp] = tokentype;
		strcpy(tkbuf[tkbufp], token);
		tkbufp++;
	}
}

int ws(void)
{
	char c;
	int rslt = NO;

	while ((c = getch()) == ' ' || c == '\t' || c == '\n')
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

void nextline(void)
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
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= MAXBUF)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
