/*

Write a program to remove all comments from a C program.  Don't forget to
handle quoted strings and character constants properly.  C comments do not
nest.

*/

#include <stdio.h>

#define ST_IN_CODE 0
#define ST_FORWARD_SLASH 1
#define ST_IN_BLOCK_COMMENT 2
#define ST_BLOCK_COMMENT_STAR 4
#define ST_IN_LINE_COMMENT 5
#define ST_IN_DOUBLE_QUOTE 6
#define ST_DOUBLE_ESCAPED 7
#define ST_IN_SINGLE_QUOTE 8
#define ST_SINGLE_ESCAPED 9

void next(int state);
void parse(int state, char c);
void unused (void);

int main () {
    next(ST_IN_CODE);
    return 0;
}

void next(int state) {
    int c;

    if((c = getchar()) == EOF)
        return;
    else
        parse(state, c);    
}

void parse(int state, char c) {
    if (state == ST_IN_CODE){
        if (c == '/') {
            next(ST_FORWARD_SLASH);
        }
        else if(c == '"') {
            putchar(c);
            next(ST_IN_DOUBLE_QUOTE);
        }
        else if(c == '\'') {
            putchar(c);
            next(ST_IN_SINGLE_QUOTE);
        }
        else {
            putchar(c);
            next(state);
        }
    }
    else if (state == ST_FORWARD_SLASH) {
        if (c == '*') {
            next(ST_IN_BLOCK_COMMENT);
        }
        else if (c == '/') {
            next (ST_IN_LINE_COMMENT);
        }
        else {
            putchar('/');
            parse(ST_IN_CODE, c);
        }
    }
    else if (state == ST_IN_BLOCK_COMMENT) {
        if (c == '*') {
            next(ST_BLOCK_COMMENT_STAR);
        }
        else {
            next(state);
        }
    }
    else if (state == ST_BLOCK_COMMENT_STAR) {
        if (c == '/') {
            next(ST_IN_CODE);
        }
        else {
            parse(ST_IN_BLOCK_COMMENT, c);
        }
    }
    else if (state == ST_IN_LINE_COMMENT) {
        if (c == '\n') {
            putchar(c);
            next(ST_IN_CODE);
        }
        else {
            next(state);
        }
    }
    else if (state == ST_IN_DOUBLE_QUOTE) {
        if (c == '"') {
            putchar(c);
            next(ST_IN_CODE);
        }
        else if (c == '\\') {
            putchar(c);
            next(ST_DOUBLE_ESCAPED);
        }
        else {
            putchar(c);
            next(state);
        }
    }
    else if (state == ST_DOUBLE_ESCAPED) {
        putchar(c);
        next(ST_IN_DOUBLE_QUOTE);
    }
    else if (state == ST_IN_SINGLE_QUOTE) {
        if (c == '\'') {
            putchar(c);
            next(ST_IN_CODE);
        }
        else if (c == '\\') {
            putchar(c);
            next(ST_SINGLE_ESCAPED);
        }
        else {
            putchar(c);
            next(state);
        }
    }
    else if (state == ST_SINGLE_ESCAPED) {
        putchar(c);
        next(ST_IN_SINGLE_QUOTE);
    }
    else{
        printf("\nError: unknwn state: %d\n", state);
    }
}

void unused (void) {
    // Single line quote
    /* Test comment with * / inside */
    "String with psudo comment here -->/* not a comment!! */<--";
    '"';
    /* Test comment with * / inside */
    "String with psudo comment here -->/* not a comment!! */<--";
    "\"";
    /* Test comment with * / inside */
    "String with psudo comment here -->/* not a comment!! */<--";
}

/*
 * cat ex1-23-remove-all-comments.c | a.out
 * (backticks manually put comment closing: *`/)
 * 

<snip>

void unused (void) {
    
    "String with psudo comment here -->/* not a comment!! *`/<--";
    '"';
    
    "String with psudo comment here -->/* not a comment!! *`/<--";
    "\"";
    
    "String with psudo comment here -->/* not a comment!! *`/<--";
}


*/
