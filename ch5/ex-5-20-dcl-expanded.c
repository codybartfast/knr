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

enum { NAME, BRACKETS };
enum { OK = 0, ERROR };

int dcl(void);
int dirdcl(void);

void nextline(void);
int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char id[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1 << 10];

int getch(void);
void ungetch(int c);
int buf[BUFSIZE];
int bufp = 0;

char printc(char c);

int ws(void);
int parseid(char *p);
int brackets(char *p);
int oparens(void);
int cparens(void);

int main(void)
{
	int rslt;

	while (gettoken() != EOF) {
		if(tokentype != NAME){
			printf("\nerror: Expected type\n");
			return 0;
		}
		strcpy(datatype, token);
		out[0] = '\0';
		if ((rslt = dcl()) != OK) {
			nextline();
			return 0;
		} else if (tokentype != ';') {
			printf("\nsytax error\n");
			nextline();
			return 0;
		} else {
			printf("\n");
			printf("%s: %s %s\n\n", id, out, datatype);
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
			if (cparens())
				strcat(out, " function returning");
			else {
				printf("\nerror: expected closking parens\n");
				return ERROR;
			}
		} else {
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

	if (oparens())
		tokentype = '(';
	else if (brackets(p))
		tokentype = BRACKETS;
	else if (parseid(p))
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

int parseid(char *p)
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

	if ((c = getch()) == '[') {
		for (*p++ = c; (*p++ = (c = getch())) != ']';)
			;
		*p = '\0';
		return 1;
	}
	ungetch(c);
	return 0;
}

int oparens(void)
{
	char c;

	if ((c = getch()) == '(')
		return 1;
	ungetch(c);
	return 0;
}

int cparens(void)
{
	char c;

	if ((c = getch()) == ')')
		return 1;
	ungetch(c);
	return 0;
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
