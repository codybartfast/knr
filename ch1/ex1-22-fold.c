/* Exercise 1-22.

Write a program to "fold" long input lines into two or more shorter lines
after the last non-blank character that occurs before the n-th column of
input. Make sure your program does something intelligent with very long
lines, and if there are no blanks or tabs before the specified column.

*/

#include <stdio.h>

#define NOTFOUND -1  /* position of blank if none found */

int maxln;
char cont;

void readln(char buff[], int pos);
void writebuff(char buff[], int pos);
int shift(char buff[], int start, int end);

int main() {
    extern int maxln;
    extern char cont;

    maxln = 77;         /* maximum length including newline > 2*/
    cont = '`';         /* line continuation character */
    char buff[maxln];

    readln(buff, 0);
}

void readln(char buff[], int pos){
    extern int maxln;
    int c, blankidx;

    blankidx = NOTFOUND;
    for (; pos < maxln && (c = getchar()) != '\n' && c != EOF; ++pos) {
        buff[pos] = c;
        if (c == ' ' || c == '\t')
            blankidx = pos;
    }
    if (c == EOF)
        writebuff(buff, pos);
    else if (c == '\n') {
        writebuff(buff, pos);
        putchar('\n');
        readln(buff, 0);
    }
    else if (blankidx != NOTFOUND){
        writebuff(buff, blankidx);
        putchar('\n');
        pos = shift(buff, blankidx + 1, pos);
        readln(buff, pos);
    }
    else{
        writebuff(buff, pos - 2);
        putchar(cont);
        putchar('\n');
        pos = shift(buff, pos - 2, pos);
        readln(buff, pos);
    }
}

void writebuff(char buff[], int pos) {
    int i;
    
    for (i = 0; i < pos; ++i)
        putchar(buff[i]);
}

int shift(char buff[], int start, int end){
    int i, j;

    for (i = start, j = 0; i < end; ++i, ++j)
        buff[j] = buff[i];
    return j;
}

/*
 * cat lorem-ipsum-long-lines.txt | a.out
 * n == 77
 * 

Lorem Ipsum
===========

Lorem ipsum dolor sit amet, consectetur  adipiscing  elit,  sed  do  eiusmod
tempor incididunt ut labore  et  dolore  magna  aliqua.  Ut  enim  ad  minim
veniam, quis nostrud exercitation ullamco laboris  nisi  ut  aliquip  ex  ea
commodo consequat.  Duis aute irure  dolor  in  reprehenderit  in  voluptate
velit esse cillum dolore eu fugiat nulla pariatur.  Excepteur sint  occaecat
cupidatat non proident, sunt in culpa qui officia deserunt  mollit  anim  id
est laborum.

--------------------\--De-finibus-bonorum-et-malorum--\--------------------`
-------------------\-Marcus-Tullius-Cicero--\--45-BCE--\--------------------

 * 
 * n == 40
 * 

Lorem Ipsum
===========

Lorem ipsum dolor sit amet, consectetur
 adipiscing  elit,  sed  do  eiusmod
tempor incididunt ut labore  et  dolore
 magna  aliqua.  Ut  enim  ad  minim
veniam, quis nostrud exercitation
ullamco laboris  nisi  ut  aliquip  ex 
ea commodo consequat.  Duis aute irure 
dolor  in  reprehenderit  in  voluptate
velit esse cillum dolore eu fugiat
nulla pariatur.  Excepteur sint 
occaecat cupidatat non proident, sunt
in culpa qui officia deserunt  mollit 
anim  id est laborum.

--------------------\--De-finibus-bono`
rum-et-malorum--\---------------------`
------------------\-Marcus-Tullius-Cic`
ero--\--45-BCE--\------------------

 *
 */
