/*
 * Exercise 7-3.
 * 
 * Revise minprintf to handle more of the other facilities of printf.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void minprintf(char *fmt, ...);

int main(void)
{
	minprintf("My favourite %s is %c\n", "char", '@');
	minprintf("My favourite %s is %d\n", "int", 73);
	minprintf("My favourite %s is %f\n", "double", 7.3);
	minprintf("My favourite %s is %hd\n", "short", -7);
	minprintf("My favourite %s is %hu\n", "unsigned short", -3);
	minprintf("My favourite %s is %ld\n", "long", -7L);
	minprintf("My favourite %s is %lu\n", "long unsigned", -3L);
	minprintf("My favourite %s is %s\n", "string", "seven");
	minprintf("My favourite %s is %u\n", "unsigned int", -3);

	return 0;
}

void minprintf(char *fmt, ...)
{
	va_list ap;
	char *p, *sval;

	char cval;
	int ival;
	double dval;
	short int sival;
	short unsigned int suval;
	long int lival;
	long unsigned luval;
	unsigned int uval;


	va_start(ap, fmt);
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		atoi(++p);
		switch (*p) {
		case 'c':
			cval = va_arg(ap, int);
			printf("%c", cval);
			break;
		case 'd':
			ival = va_arg(ap, int);
			printf("%d", ival);
			break;
		case 'f':
			dval = va_arg(ap, double);
			printf("%f", dval);
			break;
		case 'h':
			switch (*++p) {
			case 'd':
				sival = va_arg(ap, int);
				printf("%hd", sival);
				break;
			case 'u':
				suval = va_arg(ap, unsigned);
				printf("%hu", suval);
				break;
			default:
				putchar('h');
				putchar(*p);
				break;
			}
			break;
		case 'L':
			lival = va_arg(ap, long);
			printf("%li", lival);
			break;
		case 'l':
			switch (*++p) {
			case 'd':
				lival = va_arg(ap, long int);
				printf("%ld", lival);
				break;
			case 'u':
				luval = va_arg(ap, long unsigned);
				printf("%lu", luval);
				break;
			default:
				putchar('l');
				putchar(*p);
				break;
			}
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++)
				putchar(*sval);
			break;
		case 'u':
			uval = va_arg(ap, unsigned);
			printf("%u", uval);
			break;
		default:
			putchar(*p);
			break;
		}
	}
	va_end(ap);
}