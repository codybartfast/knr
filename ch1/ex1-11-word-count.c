/* Exercise 1-11

How would you test the word count program? What kinds of input are most
likely to uncover bugs if there are any?

*/

/*

Any problems are likely to happen when we have 0, 1 or 2 of 'things'. With
this implementation it is especially true as STATE is so simple it's hard to
imagine something working with 2 but not three.  So, I would test all the
variations of 0, 1, and 2 chars:

    "", " ", "X", "  ", "XX", "X ", " X"

I would try these combinations with each of the different whitespace chars.

0, 1, to 2 could then apply to the number words and the length of words and
spaces:

    "X X X"
    " X X "
    "X XX  XXX   XXXX"
    " X  XX   XXX    "

And repeat this with each of whitespace chars and mixtures of whitespace
chars.  And with different (combinations of) letters (a, z, A, Z, -).

This is probably overkill given our knowledge of the implementation, but if
we were ignorant of the implementaion then more complex solutions may result
in more subtle problems.

*/

#include <stdio.h>

#define IN 1  /* inside a word */
#define OUT 0 /* outside a word */

/* count lines, words, and characters in input */
int main()
{
    int c, nl, nw, nc, state;

    state = OUT;
    nl = nw = nc = 0;
    while ((c = getchar()) != EOF) {
        ++nc;
        if (c == '\n')
            ++nl;
        if (c == ' ' || c == '\n' || c == '\t')
            state = OUT;
        else if (state == OUT) {
            state = IN;
            ++nw;
        }
    }
    printf("%d %d %d\n", nl, nw, nc);
}

/*
 *  cat lorem-ipsum.txt | a.out
 *
14 86 590
 */