/* Exercise 1-21.

Write a program entab that replaces strings of blanks by the minimum number
of tabs and blanks to achieve the same spacing.  Use the same tab stops as
for detab.  When either a tab or a single blank would suffice to reach a tab
stop, which should be given preferene?

*/

/*

A single space is preferable to a tab.  In most contexts, such as spoken
or computer languages, a space is a sparator of words or symobols and its
absolute column is irrelevant.  Either choise will be wrong sometimes, a
space will be wrong less often.  Of course the choice could be
parameterized.

I made some attempt to handle backspaces, but I don't think it's possible
to handle them reliably without buffering complete lines which could be of
arbitrary length.

*/

#include <stdio.h>

int tsize;  /* tab size */

int dist2tab(int col);
int istabstop(int col);

int main(){
    extern int tsize;
    int col;    /* current column */
    int c, i, nspace;

    tsize = 4;
    col = nspace = 0;
    while((c = getchar()) != EOF) {
        if (c == '\b' && col > 0) {
            --col;
            if (nspace > 0)
                --nspace;
            else
                putchar(c);
        }
        else if (nspace > 0 && istabstop(col)) {
            if (nspace == 1)
                putchar(' ');
            else
                putchar('\t');
            nspace = 0;
        }
        if (c == '\t') {
            putchar(c);
            nspace = 0;
            col = col + dist2tab(col);
        }
        else if(c == ' '){
            ++col;
            ++nspace;
        }
        else if (c != '\b') {
            for (; 0 < nspace; --nspace)
                putchar(' ');
            putchar(c);
            if (c == '\n')
                col = 0;
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

int istabstop(int col)
{
    return dist2tab(col) == tsize;
}
