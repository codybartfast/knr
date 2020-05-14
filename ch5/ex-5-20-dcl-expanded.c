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

enum { NAME, PARENS, BRACKETS };
enum { OK = 0, ERROR };

int dcl(void);
int dirdcl(void);

void nextline(void);
int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1 << 10];

int getch(void);
void ungetch(int c);
int buf[BUFSIZE];
int bufp = 0;

char printc(char c);

int ws(void);
int id(char *p);
int brackets(char *p);
int oparens(char *p);
int cparens(char *p);

int main(void)
{
	int rslt;

	while (gettoken() != EOF) {
		ws();
		strcpy(datatype, token);
		out[0] = '\0';
		if ((rslt = dcl()) != OK)
			nextline();
		else if (tokentype != ';') {
			printf("sytax error\n");
			nextline();
		} else {
			printf("\n");
			printf("%s: %s %s\n\n", name, out, datatype);
		}
	}
	return 0;
}

void nextline(void)
{
	int c;

	while ((c = getch()) != '\n' && c != EOF)
		;
	if (c == EOF)
		ungetch(c);
}

int dcl(void)
{
	int ns, rslt;

	for (ns = 0; gettoken() == '*';)
		ns++;
	if ((rslt = dirdcl()) != OK)
		return rslt;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return OK;
}

int dirdcl(void)
{
	int type, rslt;

	if (tokentype == '(') {
		if ((rslt = dcl()) != OK)
			return rslt;
		if (tokentype != ')') {
			printf("error: missing )\n");
			return ERROR;
		}
	} else if (tokentype == NAME)
		strcpy(name, token);
	else {
		printf("error: expected name or (dcl)\n");
		return ERROR;
	}
	while ((type = gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
	return OK;
}

int gettoken(void)
{
	char *p = token;

	ws();

	if (oparens(p))
		if (cparens(p))
			tokentype = PARENS;
		else
			tokentype = '(';
	else if (brackets(p))
		tokentype = BRACKETS;
	else if (id(p))
		tokentype = NAME;
	else
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

int id(char *p)
{
	char c;
	int rslt = 0;

	if (isalpha(c = getch())) {
		rslt = 1;
		for (*p++ = c; isalnum(c = getch());)
			*p++ = c;
		*p = '\0';
	}
	ungetch(c);
	return rslt;
}

int brackets(char *p)
{
	char c;
	int rslt = 0;

	if ((c = getch()) == '[') {
		rslt = 1;
		for (*p++ = c; (*p++ = (c = getch())) != ']';)
			;
		*p = '\0';
	} else
		ungetch(c);
	return rslt;
}

int oparens(char *p)
{
	char c;
	int rslt = 0;

	if ((c = getch()) == '(') {
		rslt = 1;
		*p++ = c;
		*p = '\0';
	} else
		ungetch(c);
	return rslt;
}

int cparens(char *p)
{
	char c;
	int rslt = 0;

	if ((c = getch()) == ')') {
		rslt = 1;
		*p++ = c;
		*p = '\0';
	} else
		ungetch(c);
	return rslt;
}

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
