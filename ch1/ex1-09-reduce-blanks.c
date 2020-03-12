/* Exercise 1-9
 *
 * Write a program to copy its input to its output, replacing each string of
 * one or more blanks by a single blank.
 */

#include <stdio.h>

#define BLANK ' '

int main()
{
    int c, previous;
    previous = EOF;

    while ((c = getchar()) != EOF) {
        if (c != BLANK)
            putchar(c);
        if (c == BLANK)
            if (previous != BLANK)
                putchar(c);
        previous = c;
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

Lorem Ipsum
===========

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
    tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim
    veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea
    commodo consequat. Duis aute irure dolor in reprehenderit in voluptate
    velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint
    occaecat cupidatat non proident, sunt in culpa qui officia deserunt
    mollit anim id est laborum.

    \ De finibus bonorum et malorum \ Marcus Tullius Cicero \ 45 BCE \

 */