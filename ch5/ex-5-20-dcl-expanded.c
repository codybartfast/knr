/* Exercise 5-20.
 *
 * Expand dcl to handle declarations with function argument types, qualifiers
 * like const, and so on.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define SYMSIZE (1 << 10)
#define MSGSIZE (1 << 10)
#define BUFSIZE (1 << 1)
#define TKBUFSIZE (1 << 4)

enum { STORE, QUAL, TYPE, VAR, BRACKETS };
enum { OK = 0, ERROR };
enum { NO = 0, YES };

int declaration(void);
int dcl(char *name, char *out);
int dirdcl(char *name, char *out);
int params(char *out);

int gettoken(void);
void ungettoken(void);
int ws(void);
int name(char *p);
int brackets(char *p);
int oparens(void);
void nextline(void);

int getch(void);
void ungetch(int c);

int tokentype;
char token[SYMSIZE];
char dec[MSGSIZE];

int buf[BUFSIZE];
int bufp = 0;

char tkbuf[TKBUFSIZE][SYMSIZE];
int ttbuf[TKBUFSIZE];
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
	while (gettoken() != EOF) {
		if (declaration() != OK) {
			nextline();
		}
		printf("\n%s\n", dec);
	}
	return 0;
}

int declaration(void)
{
	char decspec[MSGSIZE];
	char store[SYMSIZE];
	char qual[SYMSIZE];
	char name[SYMSIZE];
	char out[MSGSIZE];

	decspec[0] = '\0';
	store[0] = '\0';
	qual[0] = '\0';
	out[0] = '\0';

	if (tokentype == STORE) {
		sprintf(store, " in %s storage", token);
		gettoken();
	}
	if (tokentype == QUAL) {
		sprintf(qual, " %s", token);
		gettoken();
	}
	do {
		if (tokentype != TYPE) {
			printf("\nerror: Expected a type\n");
			return ERROR;
		}
		if (decspec[0] != '\0')
			strcat(decspec, " ");
		strcat(decspec, token);
	} while (gettoken() == TYPE);
	ungettoken();

	if (dcl(name, out) != OK) {
		return ERROR;
	} else if (tokentype != ';' && tokentype != ',' && tokentype != ')') {
		printf("\nsytax error, got %d/%c\n", tokentype, tokentype);
		return ERROR;
	} else {
		sprintf(dec, "%s:%s%s %s%s", name, qual, out, decspec, store);
	}
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
	int argcount = 0;
	char *seperator = " argument ";

	if (gettoken() != ')') {
		do {
			if (argcount++ > 0)
				gettoken();
			if (declaration() != OK)
				return ERROR;
			strcat(out, seperator);
			strcat(out, dec);
			seperator = " and argument ";
		} while (tokentype == ',');
	}

	if (tokentype == ')') {
		if (argcount == 0)
			strcat(out, " argument : void");
		strcat(out, " returning");
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
		strcpy(token, tkbuf[tkbufp]);
		tokentype = ttbuf[tkbufp];
	} else {
		ws();
		if (!(oparens() || brackets(token) || name(token))) {
			token[0] = tokentype = getch();
			token[1] = '\0';
		}
	}
	return tokentype;
}

void ungettoken(void)
{
	if (tkbufp >= TKBUFSIZE) {
		printf("ungettoken: too many tokens\n");
	} else {
		strcpy(tkbuf[tkbufp], token);
		ttbuf[tkbufp] = tokentype;
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
	int i, rslt = NO;

	tkn = p;
	if (isalpha(c = getch())) {
		rslt = YES;
		for (*p++ = c; isalnum(c = getch());)
			*p++ = c;
		*p = '\0';
		tokentype = VAR;
		for (i = 0; i < ntypes; i++) {
			if (strcmp(tkn, types[i]) == 0) {
				tokentype = TYPE;
				break;
			}
		}
		for (i = 0; i < nstores; i++) {
			if (strcmp(tkn, stores[i]) == 0) {
				tokentype = STORE;
				break;
			}
		}
		for (i = 0; i < nquals; i++) {
			if (strcmp(tkn, quals[i]) == 0) {
				tokentype = QUAL;
				break;
			}
		}
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

int oparens(void)
{
	char c;

	if ((c = getch()) == '(') {
		token[0] = tokentype = '(';
		token[1] = '\0';
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

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
