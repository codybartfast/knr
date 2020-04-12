/* Exercise 4-1
 *
 * Write a function strrindex(s, t), which returns the position of the 
 * rightmost occurence of t in s or -1 if there is none.
 */

#include <stdio.h>

void test(char s[], char t[], int expected);
int strrindex(char s[], char t[]);

int main(void){
	test("the apple, the orange", "the", 11);
	return 0;
}

void test(char s[], char t[], int expected){
	int actual = strrindex(s, t);

	if(actual == expected){
		printf("ok: %s, %s -> %d\n", s, t, actual);
	} else {
		printf("FAIL: %s, %s -> %d (expected %d)\n", s, t,
		       actual, expected);
	}
}

int strrindex(char s[], char t[]){
	int i, j, k, rslt = -1;

	for(i = 0; s[i] != '\0'; i++) {
		for(j = i, k = 0; t[k] != '\0' && t[k] == s[j]; j++, k++)
			;
		if(k > 0 && t[k] == '\0')
			rslt = i;
	}
	return rslt;
}
