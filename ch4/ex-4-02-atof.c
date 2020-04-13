/* Exercise 4-2
 *
 * Extend atof to handle scientific notation of the form
 * 
 * 	123.45e-6
 * 
 * where a floating-point number may be followed by e or E and an optionally
 * signed exponent.
 */

#include <ctype.h>
#include <stdio.h>

void test(char s[], double expected);
double atof(char s[]);

int main(void)
{
	test("45", 45.0);
	test("45.321", 45.321);
	test("-45.321", -45.321);
	test("45.321e+0", 45.321);
	test("45.321e+1", 453.21);
	test("45.321E+2", 4532.1);
	test("45.321e-1", 4.5321);
	test("45.321E-2", 0.45321);
	return 0;
}

void test(char s[], double expected)
{
	double actual = atof(s);

	if (actual == expected) {
		printf("ok: %s -> %g\n", s, actual);
	} else {
		printf("FAIL: %s -> %g (expected %g)\n", s, actual, expected);
	}
}

double atof(char s[])
{
	double val, power;
	int i, sign, expsign, exp;

	for (i = 0; isspace(s[i]); i++)
		;
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if (s[i] == '.')
		i++;
	for (power = 1.0; isdigit(s[i]); i++) {
		val = 10.0 * val + (s[i] - '0');
		power *= 10.0;
	}
	if (s[i] == 'e' || s[i] == 'E') {
		i++;
		expsign = (s[i] == '-') ? -1 : 1;
		if (s[i] == '+' || s[i] == '-')
			i++;
		for (exp = 0; isdigit(s[i]); i++)
			exp = 10 * exp + (s[i] - '0');
		while (exp-- > 0)
			power = (expsign == 1) ? power / 10.0 : power * 10.0;
	}
	return sign * val / power;
}

/* Output

ok: 45 -> 45
ok: 45.321 -> 45.321
ok: -45.321 -> -45.321
ok: 45.321e+0 -> 45.321
ok: 45.321e+1 -> 453.21
ok: 45.321E+2 -> 4532.1
ok: 45.321e-1 -> 4.5321
ok: 45.321E-2 -> 0.45321

*/