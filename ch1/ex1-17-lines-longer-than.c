/* Exercise 1-17.
 
 Write a program to print all input lines that are longer than 80 
 characters.
 
 */

#include <stdio.h>
#define MAXLINE 1000    /* maximum input line size */
#define PRINTLIM  75    /* minimum length of lines to be printed */

int get_line(char line[], int maxline);
void copy(char to[], char from[]);

/* print longest input line */
int main()
{
    int len;                /* current line length */
    char line[MAXLINE];     /* current input line */

    printf ("-- Lines longer than %d chars --\n", PRINTLIM);
    while ((len = get_line(line, MAXLINE)) > 0)
        if (len > PRINTLIM)
            printf("%s", line);
    printf ("-- end --\n");
    return 0;
}

/* getline: read as a line into s, return length */
int get_line(char s[], int lim)
{
    int c, i;
    for (i=0; (c=getchar())!=EOF && c!='\n'; ++i)
        if (i<lim-1)
            s[i] = c;
    if (c == '\n') {
        if (i<lim-1)
            s[i] = c;
        ++i;
    }
    if (i < lim)
        s[i] = '\0';   
    else 
        s[lim-1] = '\0';
    return i;
}

/*
 *  cat lorem-ipsum.txt | a.out
-- Lines longer than 75 chars --
Lorem  ipsum  dolor sit amet,  consectetur  adipiscing elit,  sed do eiusmod
    tempor  incididunt  ut  labore et dolore magna aliqua.  Ut enim ad minim
    commodo  consequat.  Duis aute irure dolor in reprehenderit in voluptate
    velit  esse  cillum  dolore eu fugiat  nulla pariatur.   Excepteur  sint
-- end --
 */
