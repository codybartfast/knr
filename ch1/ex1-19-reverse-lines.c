/* Exercise 1-19.
 
Write a function reverse(s) that reverses the character string s.  Use it to
write a program that reverses its input a line at a time.
 
*/

/* It's not clear if the 'requirement' is for the the \n to be reversed
   along with the rest of the string.  If we're using a generic string
   reverse function then it seems \n should be included otherwise I think
   things get clunky/inefficient.  Reversln below is similar to reverse but
   excludes any terminating \n from reversal.
*/

#include <stdio.h>
#define MAXLINE 1000 /* maximum input line size */
#define NOTFOUND -1  /* value indicating index is not set */

void reverse(char to[], char from[]);
void reverseln(char to[], char from[]);
int get_line(char line[], int maxline, int trmend);

/* reverse and print lines */
int main()
{
    int len;            /* current line length */
    char line[MAXLINE]; /* current input line */
    char revd[MAXLINE]; /* current line reversed */

    while ((len = get_line(line, MAXLINE, 0)) > 0) {
        // reverse(revd, line);
        reverseln(revd, line);
        printf("%s", revd);
    }
    return 0;
}

/* reverse order of a string */
void reverse(char to[], char from[]) {
    int i, j;
    i = j = 0;
    while(from[i] != '\0')
        ++i;
    to[i] = '\0';
    while (i > 0) {
        --i;
        to[j] = from[i];
        ++j;
    }
}

/* reverse order of a line (leaves trainling \n in place) */
void reverseln(char to[], char from[])
{
    int i, j;
    i = j = 0;
    while(from[i] != '\0')
        ++i;
    to[i] = '\0';
    if (i > 0 && from[i-1] == '\n'){
        --i;
        to[i] = '\n';
    }
    while (i > 0) {
        --i;
        to[j] = from[i];
        ++j;
    }
}

/* get_line: read as a line into s, optionally trim end, return length */
int get_line(char s[], int lim, int trmend)
{
    int c, i, wsstart;
    wsstart = NOTFOUND;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i) {
        if (i < lim - 1)
            s[i] = c;
        if (trmend)
            if (c == ' ' || c == '\t') {
                if (wsstart == NOTFOUND)
                    wsstart = i;
            }
            else
                wsstart = NOTFOUND;
    }
    if (wsstart != NOTFOUND)
        i = wsstart;
    if (c == '\n') {
        if (i < lim - 1)
            s[i] = c;
        ++i;
    }
    if (i < lim)
        s[i] = '\0';
    else
        s[lim - 1] = '\0';
    return i;
}

/*
 *  cat lorem-ipsum-trailing.txt | a.out
                                                                         
                                                                 muspi merol
                                                                 ===========
                                                                         
domsuie od des  ,tile gnicsipida  rutetcesnoc  ,tema tis rolod  muspi  merol
minim da mine tu  .auqila angam erolod te erobal  tu  tnudidicni  ropmet
ae xe piuqila tu isin sirobal ocmallu noitaticrexe durtson siuq  ,mainev
etatpulov ni tiredneherper ni rolod eruri etua siud  .tauqesnoc  odommoc
tnis  ruetpecxe   .rutairap allun  taiguf ue erolod  mullic  esse  tilev
tnuresed  aiciffo iuq apluc ni tnus  ,tnediorp  non  tatadipuc  taceacco
                                             .murobal tse di mina tillom
                                                                         
\  ECB 54  \  oreciC suilluT sucraM  \  murolam te muronob subinif eD  \
                                                                         
 * (We see "muspi merol" instead "muspI meroL" because of backspaces in the
 * orignal text, i.e., "l<\b>Lorem i<\b>Ipsum").
 */
