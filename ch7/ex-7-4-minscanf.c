/*
 * Exercise 7-4.
 *
 * Write a private version of scanf analogous to minprintf from the previous
 * section.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define STRMAX (1 << 10)

int minscanf(char *str, char *fmt, ...);

int main(void)
{
	int ival = 73, matched;
	double dval = 7.3;
	char sval1[STRMAX], sval2[STRMAX];

	printf("expect 0: %d\n", minscanf("", ""));
	printf("expect 0: %d\n",
	       minscanf(" apple and banana ", "appleandbanana"));
	printf("expect 0: %d\n",
	       minscanf("I gave 110% effort", "I gave 110%% effort"));
	printf("expect 0: %d\n", minscanf("cherry coke", "cherry tree"));
	printf("expect 1: %d\n",
	       minscanf("cherry -12 coke", "cherry %d coke", &ival));
	printf("expect -12: %d\n", ival);
	printf("expect 1: %d\n", minscanf("Five nines (99.99999%) is easy!",
					  "Five nines (%f%%) is easy!", &dval));
	printf("expect 99.99999(-ish): %f\n", dval);

	matched = minscanf("First: Ben ; Last: Bitdiddle ; House number 10.",
			   "First: %s ; Last: %s ; House number %d.", sval1,
			   sval2, &ival);
	printf("expect 3: %d\n", matched);

	printf("expect 'My name is Ben Bitdiddle and I live at No.10':\n");
	printf("        My name is %s %s and I live at No.%d\n", sval1, sval2,
	       ival);
	return 0;
}

int minscanf(char *str, char *fmt, ...)
{
	va_list ap;
	int matched = 0;
	char *p;

	va_start(ap, fmt);
	for (; *fmt; fmt++) {
		if (isspace(*fmt))
			continue;
		for (; isspace(*str); str++)
			;
		if (*str == '\0')
			break;
		if (*fmt != '%') {
			if (*fmt == *str) {
				str++;
				continue;
			} else {
				break;
			}
		}

		switch (*++fmt) {
		case 'd':
			if (!isdigit(*str) && !((*str == '+' || *str == '-') &&
						isdigit(*(str + 1))))
				break;
			*va_arg(ap, int *) = atoi(str);
			str++;
			while (isdigit(*str) && *str)
				str++;
			matched++;
			continue;
		case 'f':
			*va_arg(ap, double *) = strtod(str, &p);
			if (p == str)
				break;
			str = p;
			matched++;
			continue;
		case 's':
			if (isspace(*str) || !*str)
				break;
			p = va_arg(ap, char *);
			while (!isspace(*str) && *str)
				*p++ = *str++;
			*p = '\0';
			matched++;
			continue;
		case '%':
			if (*str == '%') {
				str++;
				continue;
			} else {
				break;
			}
		default:
			break;
		}
		break;
	}
	printf("\nfmt: '%s', str: '%s'\n", fmt, str);
	va_end(ap);
	return matched;
}
