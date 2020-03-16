/*

Write a program to check a C program for rudimentary syntax errors like
unbalanced parenthesis, brackets and braces.  Don't forget about quotes,
both single and double, escape sequences and comments.  (This program is
hard if you do it in full generality.)

*/

/*
I hope this:
  1. Only uses features from Chapter 1 of K&R.
  2. Supports files/lines/statements of near arbitrary length.
  3. Does not mutate state (except for tracking line / col).
  4. Handles comments, quotes and escapes quite robustly.
  5. Checks various tags are correctly nested inside each other:
       E.g. Allows "{ ( ... ) }" but reports "{ ( ... } )".
  6. Has moderately helpful error messages.
But it's probably messy cake.
*/

#include <stdio.h>

#define MD_IN_CODE 1
#define MD_FORWARD_SLASH 2
#define MD_IN_BLOCK_COMMENT 3
#define MD_BLOCK_COMMENT_STAR 4
#define MD_IN_LINE_COMMENT 5
#define MD_IN_DOUBLE_QUOTE 6
#define MD_DOUBLE_ESCAPED 7
#define MD_IN_SINGLE_QUOTE 8
#define MD_SINGLE_ESCAPED 9

#define NOTAG '\0'
#define CONTINUE 2
#define STOP 3

int line;
int col;

int next(int mode, char exptag);
int parse(int mode, char exptag, char c);
int open_tag(int mode, char prevexptag, char closetag);
int check_closing_tag(char exptag, char acttag);
void display_eof_status(int mode, char exptag);

int main () {
    line = 1;
    col = 0;
    next(MD_IN_CODE, NOTAG);
    return 0;
}

int next(int mode, char exptag) {
    int c;

    if((c = getchar()) == EOF) {
        display_eof_status(mode, exptag);
        return STOP;
    }
    else {
        if (c == '\n') {
            ++line;
            col = 0;
        }
        else{
            ++col;
        }
        parse(mode, exptag, c);
    }
}

int parse(int mode, char exptag, char c) {
    if (mode == MD_IN_CODE){
        if (c == '(')
            open_tag(mode, exptag, ')');
        else if (c == '[')
            open_tag(mode, exptag, ']');
        else if (c == '{')
            open_tag(mode, exptag, '}');
        else if (c == ')' || c == ']' || c == '}')
            check_closing_tag(exptag, c);
        else if (c == '/')
            next(MD_FORWARD_SLASH, exptag);
        else if (c == '"')
            next(MD_IN_DOUBLE_QUOTE, exptag);
        else if (c == '\'')
            next(MD_IN_SINGLE_QUOTE, exptag);
        else
            next(mode, exptag);
    }
    else if (mode == MD_FORWARD_SLASH) {
        if (c == '*')
            next(MD_IN_BLOCK_COMMENT, exptag);
        else if (c == '/')
            next (MD_IN_LINE_COMMENT, exptag);
        else
            parse(MD_IN_CODE, exptag, c);
    }
    else if (mode == MD_IN_BLOCK_COMMENT) {
        if (c == '*')
            next(MD_BLOCK_COMMENT_STAR, exptag);
        else
            next(mode, exptag);
    }
    else if (mode == MD_BLOCK_COMMENT_STAR) {
        if (c == '/')
            next(MD_IN_CODE, exptag);
        else
            parse(MD_IN_BLOCK_COMMENT, exptag, c);
    }
    else if (mode == MD_IN_LINE_COMMENT) {
        if (c == '\n')
            next(MD_IN_CODE, exptag);
        else
            next(mode, exptag);
    }
    else if (mode == MD_IN_DOUBLE_QUOTE) {
        if (c == '"')
            next(MD_IN_CODE, exptag);
        else if (c == '\\')
            next(MD_DOUBLE_ESCAPED, exptag);
        else
            next(mode, exptag);
    }
    else if (mode == MD_DOUBLE_ESCAPED) {
        next(MD_IN_DOUBLE_QUOTE, exptag);
    }
    else if (mode == MD_IN_SINGLE_QUOTE) {
        if (c == '\'')
            next(MD_IN_CODE, exptag);
        else if (c == '\\')
            next(MD_SINGLE_ESCAPED, exptag);
        else
            next(mode, exptag);
    }
    else if (mode == MD_SINGLE_ESCAPED) {
        next(MD_IN_SINGLE_QUOTE, exptag);
    }
    else{
        printf("Internal Error: unknown mode: %d\n", mode);
    }
}

int open_tag(int mode, char prevexptag, char closetag) {
    if (next(mode, closetag) != STOP)
        return next(mode, prevexptag);
    return STOP;
}

int check_closing_tag(char exptag, char acttag) {
    if (exptag == acttag)
        return CONTINUE;

    if (exptag == NOTAG)
        printf("Error: '%c' was unexpected", acttag);
    else
        printf("Error: expected '%c' but got '%c'", exptag, acttag);
    printf(" [line:%d, col:%d]\n", line, col);
    return STOP;
}

void display_eof_status(int mode, char exptag) {
    if (mode == MD_IN_CODE && exptag == NOTAG) 
        printf("Shiny! no issues discovered :-)");
    else if (mode == MD_IN_CODE)
        printf("Error: expected '%c', but at end of file.", exptag);
    else if (mode == MD_FORWARD_SLASH)
        printf("Error: unexpected slash '/' at end of file.");
    else if (mode == MD_IN_BLOCK_COMMENT || mode == MD_BLOCK_COMMENT_STAR)
        printf("Error: open block comment '/*' at end of file.");
    else if (mode == MD_IN_DOUBLE_QUOTE || mode == MD_DOUBLE_ESCAPED)
        printf("Error: open double quote '\"' at end of file.");
    else if (mode == MD_IN_SINGLE_QUOTE || mode == MD_SINGLE_ESCAPED)
        printf("Error: open single quote ''' at end of file.");
    else
        printf("Internal Error: got mode:%d, exptag:%d", mode, exptag);
    putchar('\n');
}
