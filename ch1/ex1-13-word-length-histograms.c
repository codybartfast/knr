/* Exercise 1-13. 

Write a program to print a histogram of the lengths of words in its input. 
It is easy to draw the histogram with the bars horizontal; a vertical 
orientation is more challenging.

*/

#include <stdio.h>

#define IN 1            /* inside a word */
#define OUT 0           /* outside a word */
#define MAXLEN 99       /* maximum length of word expected/allowed */

int main()
{
    int c, i, j, state, nlet, nword, minfound, maxfound, maxfreq, count;
    int nlength[MAXLEN + 1];

    for (i = 0; i <= MAXLEN; ++i)
        nlength[i] = 0;
    state = OUT;
    nlet = nword = maxfound = maxfreq = 0;
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
            if (count > maxfreq)
                maxfreq = count;
        }
        printf("\nMinimum word length: %d, Maximum word length: %d\n", 
            minfound, maxfound);

        /* horizontal histogram */
        printf("\nFrequency of Word Lengths - Horizontal\n");
        printf("======================================\n");
        for (i = minfound; i <= maxfound; ++i){
            printf("%2d ", i);
            count = nlength[i];
            for(j = 0; j < count; ++j)
                putchar('#');
            putchar('\n');
        }

        /* vertical histogram */
        printf("\n\nFrequency of Word Lengths - Vertical\n");
        printf("====================================\n\n");
        for (i = maxfreq; i >= 1; --i) {
            printf("%2d| ", i);
            for (j = minfound; j <= maxfound; ++j)
                if (nlength[j] >= i) 
                    printf ("## ");
                else
                    printf ("   ");
            putchar('\n');
        }
        printf("  +");
        for (i = minfound; i <= maxfound; ++i)
            printf("---");
        printf("\n    ");
        for (i = minfound; i <= maxfound; ++i)
            printf("%2d ", i);
        printf("\n\n");         
    }
}

/*
 *  cat lorem-ipsum.txt | a.out
 *

Minimum word length: 1, Maximum word length: 13

Frequency of Word Lengths - Horizontal
======================================
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


Frequency of Word Lengths - Vertical
====================================

16|                   ##
15|    ##             ##
14|    ##             ##
13|    ##             ##
12|    ##             ##
11|    ##       ##    ##
10|    ##       ## ## ##
 9|    ##    ## ## ## ##
 8|    ##    ## ## ## ##
 7|    ##    ## ## ## ##
 6|    ## ## ## ## ## ##
 5|    ## ## ## ## ## ##
 4| ## ## ## ## ## ## ##    ##
 3| ## ## ## ## ## ## ## ## ## ## ##
 2| ## ## ## ## ## ## ## ## ## ## ##
 1| ## ## ## ## ## ## ## ## ## ## ## ## ##
  +---------------------------------------
     1  2  3  4  5  6  7  8  9 10 11 12 13

 */