#include <stdio.h>

/* Exercise 1-8
 * 
 * Write a program to count blanks, tabs, and newlines
 */

int main()
{
    int c;
    long cc, bc, tc, nc;
    cc = bc = tc = nc = 0;
    while ((c = getchar()) != EOF) {
        ++cc;
        if (c == ' ')
            ++bc;
        if (c == '\t')
            ++tc;
        if (c == '\n')
            ++nc;
    }

    printf(
        "Chars: %ld, blank: %ld, tabs: %ld, newlines: %ld\n", 
        cc, bc, tc, nc);
}

/*
 * Using:
 * 
 *     cat lorem-ipsum.txt | a.out
 *  
 * to feed the input file to the program produced the output:
 * 
 *     Chars: 590, blank: 106, tabs: 7, newlines: 14
 */