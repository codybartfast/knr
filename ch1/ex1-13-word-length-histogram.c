/* Exercise 1-13. 

Write a program to print a histogram of the lengths of words in its input. 
It is easy to draw the histogram with the bars horizontal; a vertical 
orientation is more challenging.

*/

#include <stdio.h>

#define IN 1        /* inside a word */
#define OUT 0       /* outside a word */
#define MAXLEN 99   /* maximum length of word expected/allowed */
#define BARCHAR '#' /* character to represent bars on chart */

int main()
{
    int c, i, j, state, nlet, nword, minfound, maxfound, count;
    int nlength[MAXLEN + 1];

    for (i = 0; i <= MAXLEN; ++i)
        nlength[i] = 0;
    state = OUT;
    nlet = nword = maxfound = 0;
    minfound = MAXLEN;

    /* collect data on length frequency */
    while ((c = getchar()) != EOF) {
        if (c != ' ' && c != '\n' && c != '\t') {
            state = IN;
            ++nlet;
        }
        else if (state == IN) {
            if (nlet > MAXLEN)
                /* I guess we'll learn how to handle errors later. */;
            ++nlength[nlet];
            nlet = 0;
            ++nword;
            state = OUT;
        }
    }

    // for(i = 0; i <= MAXLEN; ++i)
    //     printf(" %d", nlength[i]);

    /* find min and max word lengths */
    if(nword == 0)
        printf("no words found.\n");
    else
    {
        for (i = 0; i <= MAXLEN; ++i) {
            count = nlength[i];
            if (count > 0)
                if (i < minfound)
                    minfound = i;
                else if (i > maxfound)
                    maxfound = i;
        }
        printf("Minimum word length: %d, Maximum word length: %d\n", 
            minfound, maxfound);

        /* horizontal histogram */
        printf("\nFrequency of Word Lengths\n");
        printf("=========================\n");
        for (i = minfound; i <= maxfound; ++i){
            printf("%2d ", i);
            count = nlength[i];
            for(j = 0; j < count; ++j)
                putchar(BARCHAR);
            putchar('\n');
        }

    }
}

/*
 *  cat lorem-ipsum.txt | a.out
 *
Minimum word length: 1, Maximum word length: 13

Frequency of Word Lengths
=========================
 1 ####
 2 ###############
 3 ######
 4 #########
 5 ###########
 6 ##########
 7 ################
 8 ###
 9 ####
10 ###
11 ###
12 #
13 #
 */