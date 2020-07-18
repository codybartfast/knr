#include <stdio.h>
#define LEN 1000

int main(void)
{
	char line[LEN];
	int ln = 0;

	while (fgets(line, LEN, stdin))
		printf("%4d: %s", ++ln, line);
	return 0;
} /* intentionally no final \n */