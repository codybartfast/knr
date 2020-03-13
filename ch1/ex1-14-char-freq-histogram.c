/* Exercise 1-14.

Write a program to print a histogram of the frequencies of different
characters in its output.

*/

#include <stdio.h>

#define CHARSETLEN 128  /* Size of the character set */
#define IGNORECASE 1    /* ignore letter casing */

int main()
{
    int c, i, j, nchar, maxfreq, lower;
    nchar = maxfreq = 0;

    int charfreq[CHARSETLEN];
    for (i = 0; i < CHARSETLEN; ++i)
        charfreq[i] = 0;
    
    /* collect data on char frequency */
    while ((c = getchar()) != EOF) {
        ++nchar;
        ++charfreq[c];
    }

    /* combine upper case count with lower case count */
    if (IGNORECASE) {   
        for (i = 'A'; i <= 'Z'; ++i) {
            lower = i+('a'-'A');
            charfreq[lower] = charfreq[lower]+charfreq[i];
            charfreq[i] = 0;
        }
    }

    if(nchar == 0)
        printf("no chars found.\n");
    else
    {
        for (i = 0; i <= CHARSETLEN; ++i) {            
            if (charfreq[i] > maxfreq)
                maxfreq = charfreq[i];
        }

        printf("\nCharacter Frequencies (case insensitive)\n");
        printf("========================================\n\n");

        for (i = maxfreq; i >= 1; --i) {
            printf("%3d| ", i);
            for (j = 0; j < CHARSETLEN; ++j)
                if(charfreq[j] > 0)
                    if (charfreq[j] >= i) 
                        printf ("# ");
                    else
                        printf ("  ");
            putchar('\n');
        }
        printf("   +");
        for (i = 0; i < CHARSETLEN; ++i)
            if(charfreq[i] > 0)
                printf("--");
        printf("\n    ");
        for (i = 0; i < CHARSETLEN; ++i)
            if (charfreq[i] > 0)
                if (i == '\b')
                    printf("\\b");
                else if (i == '\t')
                    printf("\\t");
                else if (i == '\n')
                    printf("\\n");
                else if (i == ' ')
                    printf("  ");
                else
                    printf(" %c", i);
        printf("\n\n");         
    }
}

/*
 *  cat lorem-ipsum.txt | a.out
 *

Character Frequencies (case insensitive)
========================================

106|       #                                                       
105|       #                                                       
104|       #                                                       
<snip>
 49|       #                                                       
 48|       #                             #                         
 47|       #                             #                         
 46|       #                             #                         
 45|       #                             #                         
 44|       #                     #       #                         
 43|       #                     #       #                         
 42|       #                     #       #                         
 41|       #                     #       #                         
 40|       #                     #       #                         
 39|       #                     #       #                         
 38|       #                     #       #                         
 37|       #                     #       #                   #     
 36|       #                     #       #                   #     
 35|       #                     #       #                   #     
 34|       #                     #       #       #         # #     
 33|       #                     #       #       #         # #     
 32|       #                     #       #       #         # #     
 31|       #             #       #       #       #         # #     
 30|       #             #       #       #       #         # #     
 29|       #             #       #       #       #         # #     
 28|       #             #       #       # #     #         # #     
 27|       #             #       #       # #     #     #   # #     
 26|       #             #       #       # #   # #     #   # #     
 25|       #             #       #       # #   # #     #   # #     
 24|       #             #       #       # #   # #     #   # #     
 23|       #             #       #       # # # # #     #   # #     
 22|       #             #       #       # # # # #     # # # #     
 21|       #             #     # #       # # # # #     # # # #     
 20|       #             #   # # #       # # # # #     # # # #     
 19|       #             #   # # #       # # # # #     # # # #     
 18|       #             #   # # #       # # # # #     # # # #     
 17|       #             #   # # #       # # # # #     # # # #     
 16|       #             #   # # #       # # # # #     # # # #     
 15|       #             #   # # #       # # # # #     # # # #     
 14|     # #             #   # # #       # # # # #     # # # #     
 13|     # #             #   # # #       # # # # #     # # # #     
 12|     # #             #   # # #       # # # # # #   # # # #     
 11|     # #         #   #   # # #       # # # # # #   # # # #     
 10|     # #         #   #   # # #       # # # # # #   # # # #     
  9|     # #         #   #   # # #       # # # # # #   # # # #     
  8|     # #         #   #   # # #       # # # # # #   # # # #     
  7|   # # #         #   #   # # #       # # # # # #   # # # #     
  6| # # # #         #   # # # # #       # # # # # #   # # # #     
  5| # # # #         #   # # # # #       # # # # # # # # # # #     
  4| # # # # # #     # # # # # # # #     # # # # # # # # # # #     
  3| # # # # # #     # # # # # # # # #   # # # # # # # # # # # # # 
  2| # # # # # #     # # # # # # # # #   # # # # # # # # # # # # # 
  1| # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
   +--------------------------------------------------------------
    \b\t\n   , . 4 5 = \ a b c d e f g h i l m n o p q r s t u v x

 */
