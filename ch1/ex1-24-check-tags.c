/*

Write a program to check a C program for rudimentary syntax errors like
unbalanced parenthesis, brackets and braces.  Don't forget about quotes,
both single and double, escape sequences and comments.  (This program is
hard if you do it in full generality.)

*/

/*
This ONLY checks that tags are balanced and correctly nested.  It assumes
the contents of comments and quotes can be ignored.  However, a modified
version of exercise 1-23 can replace the contents of comments and quotes
with spaces, together they can be used like this:

    cat source.c | ex23replace | ex24

E.g., if tags are properly nested/balanced:
*/
//      ch1> cat source1.c
//        [{  ([{(   /*]**/   ']'   '\''   "\""   "]"   )}])  }]
//      ch1> cat source1.c | ex23replace
//        [{  ([{(   /*  */   ' '   '  '   "  "   " "   )}])  }]
//      ch1> cat source1.c | ex23replace | ex24
//      Shiny! no issues discovered :-)
//      ch1>
//
//  and if the tags are not properly nested/balanced (first two swapped):
//
//      ch1> cat source2.c
//        {[  ([{(   /*]**/   ']'   '\''   "\""   "]"   )}])  }]
//      ch1> cat source2.c | ex23replace
//        {[  ([{(   /*  */   ' '   '  '   "  "   " "   )}])  }]
//      ch1> cat source2.c | ex23replace | ex24
//      Error: expected ']' but got '}' [line:1, col:55]
//      ch1>
/*
    The contents of comments are replaced (instead of removing the comments)
    to prevent the line and column values changing.
*/

#include <stdio.h>

#define NOTAG '\0'
#define CONTINUE 2
#define STOP 3

int line;
int col;

int next(char exptag);
int parse(char exptag, char c);
int open_tag(char prevexptag, char closetag);
int check_closing_tag(char exptag, char acttag);
void report_eof_status(char exptag);

int main () {
    line = 1;
    col = 0;
    next(NOTAG);
    return 0;
}

int next(char exptag) {
    int c;

    if((c = getchar()) == EOF) {
        report_eof_status(exptag);
        return STOP;
    }
    else if (c == '\n') { ++line; col = 0; }
    else { ++col; }
    parse(exptag, c);
}

int parse(char exptag, char c) {
    if (c == '(')
        open_tag(exptag, ')');
    else if (c == '[')
        open_tag(exptag, ']');
    else if (c == '{')
        open_tag(exptag, '}');
    else if (c == ')' || c == ']' || c == '}')
        check_closing_tag(exptag, c);
    else
        next(exptag);
}

int open_tag(char prevexptag, char closetag) {
    if (next(closetag) != STOP)
        return next(prevexptag);
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

void report_eof_status(char exptag) {
    if (exptag == NOTAG)
        printf("Shiny! no issues discovered :-)");
    else
        printf("Error: expected '%c', but at end of file.", exptag);
    putchar('\n');
}
