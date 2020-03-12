#include <stdio.h>

/*
 * If we have "while (c = getchar() != EOF)" then, when getchar returns EOF,
 * c is 'false' and the while loop stops before the char can be put.
 *   
 * ctl-d can be used to send an EOF character.
 * ctl-c can be used to stop the program.
 */

int main()
{
    int c;
    while (1) {
        c = getchar() != EOF;
        printf("%d\n", c);
    }
}
