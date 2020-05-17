/* Exercise 5-20.
 *
 * Expand dcl to handle declarations with function argument types, qualifiers
 * like const, and so on.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define BUFSIZE (1 << 10)

enum { NAME, TYPE, BRACKETS };
enum { OK = 0, ERROR };
enum { YES = 0, NO };

int declaration(int reqname);

int dcl(int reqname);
int dirdcl(int reqname);

void nextline(void);
int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char id[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1 << 10];

char printc(char c);
int getch(void);
void ungetch(int c);
int buf[BUFSIZE];
int bufp = 0;

char *types[] = { "void",  "char",   "short",  "int",	  "long",
		  "float", "double", "signed", "unsigned" };
int ntypes = 9;
int params(void);
int ws(void);
int parseid(char *p);
int brackets(char *p);
int oparens(void);
int cparens(void);

int main(void)
{
	while (gettoken() != EOF)
		if (declaration(YES) != OK)
			nextline();
	return 0;
}

int declaration(int reqname)
{
	if (tokentype != TYPE) {
		printf("\nerror: Expected a type\n");
		return ERROR;
	}
	strcpy(datatype, token);
	out[0] = '\0';
	if (dcl(reqname) != OK) {
		return ERROR;
	} else if (tokentype != ';') {
		printf("\nsytax error\n");
		return ERROR;
	} else {
		printf("\n");
		printf("%s: %s %s\n\n", id, out, datatype);
	}
	return OK;
}

void nextline(void)
{
	int c;

	while ((c = getch()) != ';' && c != EOF)
		;
	if (c == EOF)
		ungetch(c);
}

int dcl(int reqname)
{
	int ns, rslt;

	for (ns = 0; gettoken() == '*';)
		ns++;
	if ((rslt = dirdcl(reqname)) != OK)
		return rslt;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return OK;
}

int dirdcl(int reqname)
{
	int type, rslt;

	if (tokentype == '(') {
		if ((rslt = dcl(reqname)) != OK)
			return rslt;
		if (tokentype != ')') {
			printf("\nerror: missing )\n");
			return ERROR;
		}
	} else if (tokentype == NAME)
		strcpy(id, token);
	else {
		printf("\nerror: expected name or (dcl)\n");
		return ERROR;
	}
	while ((type = gettoken()) == '(' || type == BRACKETS)
		if (type == '(') {
			strcat(out, " function taking");
			if ((rslt = params()) != 0) {
				return rslt;
			}
		} else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
	return OK;
}

int params(void)
{
	int argcount = 0;
	if (gettoken() != ')') {
		do {
			argcount++;
		} while (gettoken() == ',');
	}
	if (argcount == 0)
		strcat(out, " no arguments");
	if (tokentype == ')')
		strcat(out, " and returning");
	else {
		printf("\nerror: expected closing parentheses "
		       "after paramaters\n");
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
		tokentype = NAME;
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

char printc(char c)
{
	if (c != EOF && c != '\n')
		printf("%c", c);
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
