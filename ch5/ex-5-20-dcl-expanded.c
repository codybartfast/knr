/* Exercise 5-20.
 *
 * Expand dcl to handle declarations with function argument types, qualifiers
 * like const, and so on.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define SYMSIZE (1 << 10)
#define MSGSIZE (1 << 10)
#define BUFSIZE (1 << 1)

enum { TYPE, VAR, BRACKETS };
enum { OK = 0, ERROR };
enum { NO = 0, YES };

int declaration(int reqname);

int dcl(char *name, char *out, int reqname);
int dirdcl(char *name, char *out, int reqname);

void nextline(void);
int gettoken(void);
int tokentype;
char token[SYMSIZE];
/* char name[SYMSIZE]; */
/* char out[MSGSIZE]; */
char dec[MSGSIZE];

char printc(char c);
int getch(void);
void ungetch(int c);
int buf[BUFSIZE];
int bufp = 0;

char *types[] = { "void",  "char",   "short",  "int",	  "long",
		  "float", "double", "signed", "unsigned" };
int ntypes = 9;
int params(char *out);
int ws(void);
int parseid(char *p);
int brackets(char *p);
int oparens(void);
int cparens(void);

int main(void)
{
	while (gettoken() != EOF) {
		/* out[0] = '\0'; */
		if (declaration(YES) != OK) {
			nextline();
			return 0; /* for testing */
		}
		printf("\nTADA: %s\n\n", dec);
	}
	return 0;
}

int declaration(int reqname)
{
	char *datatype;
	char name[SYMSIZE];
	char out[MSGSIZE];

	out[0] = '\0';

	if (tokentype != TYPE) {
		printf("\nerror: Expected a type\n");
		return ERROR;
	}
	datatype = malloc((strlen(token) + 1) * sizeof(char));
	strcpy(datatype, token);
	if (dcl(name, out, reqname) != OK) {
		return ERROR;
	} else if (tokentype != ';' && tokentype != ',' && tokentype != ')') {
		printf("\nsytax error, got %d/%c\n", tokentype, tokentype);
		return ERROR;
	} else {
		sprintf(dec, "%s:%s %s", name, out, datatype);
	}
	return OK;
}

int dcl(char *name, char *out, int reqname)
{
	int ns, rslt;

	for (ns = 0; gettoken() == '*';)
		ns++;
	if ((rslt = dirdcl(name, out, reqname)) != OK)
		return rslt;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return OK;
}

int dirdcl(char *name, char *out, int reqname)
{
	int rslt;
	int gotnext = NO;

	if (tokentype == '(') {
		if ((rslt = dcl(name, out, reqname)) != OK)
			return rslt;
		if (tokentype != ')') {
			printf("\nerror: missing )\n");
			return ERROR;
		}
	} else if (tokentype == VAR)
		strcpy(name, token);
	else if (!reqname) {
		printf("\n-- implicit name -- (%d/%c)\n", tokentype, tokentype);
		reqname = YES;
		name[0] = '\0';
		gotnext = YES;
	} else {
		printf("\nerror: expected variable name or (dcl)\n");
		return ERROR;
	}
	if (!gotnext)
		gettoken();
	while (tokentype == '(' || tokentype == BRACKETS) {
		if (tokentype == '(') {
			strcat(out, " function taking");
			if ((rslt = params(out)) != 0) {
				return rslt;
			}
			/* printf("\nback from params with %s\n", out); */
		} else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
		gettoken();
	}
	return OK;
}

int params(char *out)
{
	int argcount = 0;
	char *seperator = " argument ";
	/* printf("\nparams, out: %s\n", out); */

	if (gettoken() != ')') {
		do {
			argcount++;
			if (tokentype == ',')
				gettoken();
			if (declaration(NO) != OK)
				return ERROR;
			/* printf("\nadding dec: %s\n", dec); */
			strcat(out, seperator);
			strcat(out, dec);
			seperator = " and argument ";
		} while (tokentype == ',');
		/* printf("\nargcount %d, blah = %d/%c", argcount, tokentype,
		       tokentype); */
	}

	if (tokentype == ')') {
		if (argcount == 0)
			strcat(out, " argument : void");
		strcat(out, " and returning");
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
	char *p = token;

	ws();

	if (!(oparens() || brackets(p) || parseid(p)))
		tokentype = getch();
	/* printf("returning tkntype: %d/%c\n", tokentype, tokentype); */
	return tokentype;
}

int ws(void)
{
	char c;
	int rslt = 0;

	while ((c = getch()) == ' ' || c == '\t' || c == '\n')
		rslt = 1;
	ungetch(c);
	return rslt;
}

int parseid(char *p)
{
	char c, *tkn;
	int i, rslt = 0;

	tkn = p;
	if (isalpha(c = getch())) {
		rslt = 1;
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
		return 1;
	}
	ungetch(c);
	return 0;
}

int oparens(void)
{
	char c;

	if ((c = getch()) == '(') {
		tokentype = '(';
		return 1;
	}
	ungetch(c);
	return 0;
}

/* int cparens(void)
{
	char c;

	if ((c = getch()) == ')') {
		tokentype = ')';
		return 1;
	}
	ungetch(c);
	return 0;
} */

void nextline(void)
{
	int c;

	while ((c = getch()) != ';' && c != EOF)
		;
	if (c == EOF)
		ungetch(c);
}

char printc(char c)
{
/* 	if (c != EOF && c != '\n')
		printf("%c", c); */
	return c;
}

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : printc(getchar());
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}