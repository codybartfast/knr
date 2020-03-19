/*
Write a program to remove all comments from a C program.  Don't forget to
handle quoted strings and character constants properly.  C comments do not
nest.
*/

/*
This has options to change the behaviour so that:

    1. the contents of comments can be replaced (e.g. with spaces) instead
       of removing the comment completely.
    2. similarly the contents of single and double quotes can be replaced.
    3. reporting of errors can be enabled (e.g. reaching the end of the file
       without an open comment).
       
This is so it can be used as part of the solution to Exercise 1-24.
*/

#include <stdio.h>

#define MD_IN_CODE 0
#define MD_FORWARD_SLASH 1
#define MD_IN_BLOCK_COMMENT 2
#define MD_BLOCK_COMMENT_STAR 4
#define MD_IN_LINE_COMMENT 5
#define MD_IN_DOUBLE_QUOTE 6
#define MD_DOUBLE_ESCAPED 7
#define MD_IN_SINGLE_QUOTE 8
#define MD_SINGLE_ESCAPED 9

#define NO 0
#define YES 1

#define CMNT_INCLUDE 2
/* this comment becomes --> */		/* this comment becomes --> */

#define CMNT_REPLACE 3
/* this comment becomes --> */ 		/*                          */

#define CMNT_REMOVE 4
/* this comment becomes --> */

int cmnt_policy = CMNT_REMOVE;
char cmntd_rplcmnnt = ' ';
int do_rplc_dblqtd = NO;
char dblqtd_rplcmnt = ' ';
int do_rplc_snglqtd = NO;
char snglqtd_rplcmnt = ' ';
int do_rpt_errors = YES;

void next(int mode);
void parse(int mode, char c);
void put_cmntd_char(char c);
void put_dbl_qtd_char(char c);
void put_sngl_qtd_char(char c);
void report_eof_status(int mode);

int main(void)
{
	next(MD_IN_CODE);
	return 0;
}

void next(int mode)
{
	int c;

	if ((c = getchar()) == EOF) {
		if (do_rpt_errors)
			report_eof_status(mode);
		return;
	} else {
		parse(mode, c);
	}
}

void parse(int mode, char c)
{
	if (mode == MD_IN_CODE) {
		if (c == '/') {
			next(MD_FORWARD_SLASH);
		} else if (c == '"') {
			putchar(c);
			next(MD_IN_DOUBLE_QUOTE);
		} else if (c == '\'') {
			putchar(c);
			next(MD_IN_SINGLE_QUOTE);
		} else {
			putchar(c);
			next(mode);
		}
	} else if (mode == MD_FORWARD_SLASH) {
		if (c == '*') {
			if (cmnt_policy != CMNT_REMOVE)
				printf("/*");
			next(MD_IN_BLOCK_COMMENT);
		} else if (c == '/') {
			if (cmnt_policy != CMNT_REMOVE)
				printf("//");
			next(MD_IN_LINE_COMMENT);
		} else {
			putchar('/');
			parse(MD_IN_CODE, c);
		}
	} else if (mode == MD_IN_BLOCK_COMMENT) {
		if (c == '*') {
			next(MD_BLOCK_COMMENT_STAR);
		} else {
			put_cmntd_char(c);
			next(mode);
		}
	} else if (mode == MD_BLOCK_COMMENT_STAR) {
		if (c == '/') {
			if (cmnt_policy != CMNT_REMOVE)
				printf("*/");
			next(MD_IN_CODE);
		} else {
			put_cmntd_char('*');
			parse(MD_IN_BLOCK_COMMENT, c);
		}
	} else if (mode == MD_IN_LINE_COMMENT) {
		if (c == '\n') {
			putchar(c);
			next(MD_IN_CODE);
		} else {
			put_cmntd_char(c);
			next(mode);
		}
	} else if (mode == MD_IN_DOUBLE_QUOTE) {
		if (c == '"') {
			putchar(c);
			next(MD_IN_CODE);
		} else if (c == '\\') {
			put_dbl_qtd_char(c);
			next(MD_DOUBLE_ESCAPED);
		} else {
			put_dbl_qtd_char(c);
			next(mode);
		}
	} else if (mode == MD_DOUBLE_ESCAPED) {
		put_dbl_qtd_char(c);
		next(MD_IN_DOUBLE_QUOTE);
	} else if (mode == MD_IN_SINGLE_QUOTE) {
		if (c == '\'') {
			putchar(c);
			next(MD_IN_CODE);
		} else if (c == '\\') {
			put_sngl_qtd_char(c);
			next(MD_SINGLE_ESCAPED);
		} else {
			put_sngl_qtd_char(c);
			next(mode);
		}
	} else if (mode == MD_SINGLE_ESCAPED) {
		put_sngl_qtd_char(c);
		next(MD_IN_SINGLE_QUOTE);
	} else {
		printf("\n\nError: unknown mode: %d\n", mode);
	}
}

void put_cmntd_char(char c)
{
	if (cmnt_policy == CMNT_REMOVE)
		/* don't put anything */;
	else if (cmnt_policy == CMNT_REPLACE) {
		if (c == '\n' || c == '\t')
			putchar(c);
		else
			putchar(cmntd_rplcmnnt);
	} else
		putchar(c);
}

void put_dbl_qtd_char(char c)
{
	if (do_rplc_dblqtd == YES)
		putchar(dblqtd_rplcmnt);
	else
		putchar(c);
}

void put_sngl_qtd_char(char c)
{
	if (do_rplc_snglqtd == YES)
		putchar(snglqtd_rplcmnt);
	else
		putchar(c);
}

void report_eof_status(int mode)
{
	printf("\n\n/* ");
	if (mode == MD_IN_CODE)
		printf("No comment or quote issues discovered.");
	else {
		printf("[) ");
		if (mode == MD_FORWARD_SLASH)
			printf("Error: unexpected slash '/' at end of file.");
		else if (mode == MD_IN_BLOCK_COMMENT ||
			 mode == MD_BLOCK_COMMENT_STAR)
			printf("Error: open comment '/*' at end of file.");
		else if (mode == MD_IN_DOUBLE_QUOTE ||
			 mode == MD_DOUBLE_ESCAPED)
			printf("Error: open double quote '\"' at end of file.");
		else if (mode == MD_IN_SINGLE_QUOTE ||
			 mode == MD_SINGLE_ESCAPED)
			printf("Error: open single quote ''' at end of file.");
		else
			printf("Internal Error: got mode:%d", mode);
	}
	printf(" */\n");
}
