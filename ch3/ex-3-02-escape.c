/*
 * Exercise 3-2
 *
 * Write a function escape(s, t) that converts characters like newline and
 * tab into visible escape sequences like \n and \t as it copies the string
 * to to s.  Use a switch.  Write a function for the other direction as
 * well, converting escape sequences into the real characers.
 */

#include <stdio.h>

enum bool { NO, YES };

void escape(char s[], char t[]);
char esc_char(char c);
void unescape(char s[], char t[]);
char unesc_char(char c);

int main(void)
{
	char s[] = "Ln 1:\t\"Hello\", 'World!'\n           Ln 2:\tC:\\Windows";
	char e[100];
	char u[100];

	escape(s, e);
	unescape(e, u);

	printf("orignal:   %s\n\n", s);
	printf("escaped:   %s\n\n", e);
	printf("unescaped: %s\n\n", u);
	return 0;
}

void escape(char s[], char t[])
{
	int i = 0, j = 0;
	char c, e;

	while ((c = s[i++])) {
		if ((e = esc_char(c))) {
			t[j++] = '\\';
			t[j++] = e;
		} else {
			t[j++] = c;
		}
	}
	t[j] = '\0';
}

char esc_char(char c)
{
	switch (c) {
	case '\\':
		return '\\';
	case '\n':
		return 'n';
	case '\r':
		return 'r';
	case '\t':
		return 't';
	case '"':
		return '"';
	default:
		return '\0';
	}
}

void unescape(char s[], char t[])
{
	int i = 0, j = 0, esc_mode = NO;
	char c;

	while ((c = s[i++])) {
		if (esc_mode) {
			t[j++] = unesc_char(c);
			esc_mode = NO;
		} else if (c == '\\') {
			esc_mode = YES;
		} else {
			t[j++] = c;
		}
	}
	t[j] = '\0';
}

char unesc_char(char c)
{
	switch (c) {
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	case '\\':
	case '"':
	default:
		return c;
	}
}

/*
Output
======

orignal:   Ln 1:    "Hello", 'World!'
           Ln 2:    C:\Windows

escaped:   Ln 1:\t\"Hello\", 'World!'\n           Ln 2:\tC:\\Windows

unescaped: Ln 1:    "Hello", 'World!'
           Ln 2:    C:\Windows

*/
