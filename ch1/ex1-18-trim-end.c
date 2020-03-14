/* Exercise 1-18.
 
 Write a program to remove trailing blanks and tabs from each line of input,
 and to delete entirely blank lines.
 
 */

#include <stdio.h>
#define MAXLINE 1000 /* maximum input line size */
#define NOTFOUND -1  /* value indicating index is not set */

int get_line(char line[], int maxline, int trmend);

/* print longest input line */
int main()
{
    int len;            /* current line length */
    char line[MAXLINE]; /* current input line */

    while ((len = get_line(line, MAXLINE, 1)) > 0)
        if (line[0] != '\n')
            printf("[len:%2d]%s", len, line);
    return 0;
}

/* getline: read as a line into s, optionally trim end, return length */
int get_line(char s[], int lim, int trmend)
{
    int c, i, wsstart;
    wsstart = NOTFOUND;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i) {
        if (i < lim - 1)
            s[i] = c;
        if (trmend)
            if (c == ' ' || c == '\t') {
                if (wsstart == NOTFOUND)
                    wsstart = i;
            }
            else
                wsstart = NOTFOUND;
    }
    if (wsstart != NOTFOUND)
        i = wsstart;
    if (c == '\n') {
        if (i < lim - 1)
            s[i] = c;
        ++i;
    }
    if (i < lim)
        s[i] = '\0';
    else
        s[lim - 1] = '\0';
    return i;
}

/*
 *  cat lorem-ipsum-trailing.txt | a.out
[len:16]Lorem Ipsum
[len:12]===========
[len:79]Lorem  ipsum  dolor sit amet,  consectetur  adipiscing elit,  sed do eiusmod
[len:76]    tempor  incididunt  ut  labore et dolore magna aliqua.  Ut enim ad minim
[len:74]    veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea
[len:76]    commodo  consequat.  Duis aute irure dolor in reprehenderit in voluptate
[len:76]    velit  esse  cillum  dolore eu fugiat  nulla pariatur.   Excepteur  sint
[len:74]    occaecat  cupidatat  non  proident,  sunt in culpa qui officia  deserunt
[len:29]    mollit anim id est laborum.
[len:74]    \  De finibus bonorum et malorum  \  Marcus Tullius Cicero  \  45 BCE  \
 */
