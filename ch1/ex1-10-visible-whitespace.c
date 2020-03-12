/* Exercise 1-9

Write a program to copy its input to its output, replacing each tab by \t,
and backspace by \b and each backslash by \\.  This makes tabs and
backspaces visible in an unambiguous way.

 */

#include <stdio.h>

int main()
{
    int c;

    while ((c = getchar()) != EOF) {
        if (c != '\t')
            if (c != '\b')
                if (c != '\\')
                    putchar(c);
        if (c == '\t')
            printf("\\t");
        if (c == '\b')
            printf("\\b");
        if (c == '\\')
            printf("\\\\");
    }
}

/*
 * cat lorem-ipsum.txt
 *

Lorem Ipsum
===========

Lorem  ipsum  dolor sit amet,  consectetur  adipiscing elit,  sed do eiusmod
    tempor  incididunt  ut  labore et dolore magna aliqua.  Ut enim ad minim
    veniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea
    commodo  consequat.  Duis aute irure dolor in reprehenderit in voluptate
    velit  esse  cillum  dolore eu fugiat  nulla pariatur.   Excepteur  sint
    occaecat  cupidatat  non  proident,  sunt in culpa qui officia  deserunt
    mollit anim id est laborum.

    \  De finibus bonorum et malorum  \  Marcus Tullius Cicero  \  45 BCE  \

 *
 *  cat lorem-ipsum.txt | a.out
 *

l\bLorem i\bIpsum
===========

l\bLorem  ipsum  dolor sit amet,  consectetur  adipiscing elit,  sed do eiusmod
\ttempor  incididunt  ut  labore et dolore magna aliqua.  u\bUt enim ad minim
\tveniam,  quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea
\tcommodo  consequat.  d\bDuis aute irure dolor in reprehenderit in voluptate
\tvelit  esse  cillum  dolore eu fugiat  nulla pariatur.   e\bExcepteur  sint
\toccaecat  cupidatat  non  proident,  sunt in culpa qui officia  deserunt
\tmollit anim id est laborum.

\t\\  De finibus bonorum et malorum  \\  Marcus Tullius Cicero  \\  45 BCE  \\

 */
