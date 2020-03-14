/* Exercise 1-20.

Write a program that replaces tabs in the input with the proper number of
blanks to space to the next tab stop.  Assume a fixed set of tab stops, say
every n columns.  Should n be a variable or a sybolic parameter?

*/

/* The size of tab should be a variable so we can use different sizes for
   different inputs without having to recompile (which would need some way
   to provide that argument to the program), or perhaps we might try and
   analyze the input to guess the tab size.
*/

#include <stdio.h>

int tsize;  /* tab size */

int dist2tab(int col);

int main(){
    extern int tsize;
    int col;    /* current column */
    int c, i;

    tsize = 4;
    col = 0;
    while((c = getchar()) != EOF) {
        if(c == '\t') {
            for(i = dist2tab(col); i > 0; --i) {
                putchar(' ');
                ++col;
            }
        }
        else {
            putchar(c);
            if (c == '\n')
                col = 0;
            else if (c == '\b' && col > 0)
                --col;
            else
                ++col;
        }
    }
}

int dist2tab(int col)
{
    while(col >= tsize)
        col = col - tsize;
    return (tsize - col);
}
