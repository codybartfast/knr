/* Exercise 5-18.
 *
 * Make dcl recover from input errors.
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

int main(void)
{
	int rslt;

	while (gettoken() != EOF) {
		strcpy(datatype, token);
		out[0] = '\0';
		if ((rslt = dcl()) != OK)
			nextline();
		else if (tokentype != '\n') {
			printf("sytax error\n");
			nextline();
		} else
			printf("%s: %s %s\n", name, out, datatype);
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
	int c;
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getch()) != ']';)
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch());)
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;

	} else {
		return tokentype = c;
	}
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
