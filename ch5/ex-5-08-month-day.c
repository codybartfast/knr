/* Exercise 5-8.
 * 
 * There is no error checking in day_of_year or month_day.
 * Remedy this defect.
 */

#include <stdio.h>

#define ERROR -1

void test2yday(int year, int month, int day, int expected);
void test2month(int year, int day, int exmonth, int exday);
int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

static char daytab[2][13] = {
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

int main(void)
{
	test2yday(2020, 4, 24, 115);
	test2yday(2020, 12, 31, 366);
	test2yday(2021, 12, 31, 365);

	test2yday(2020, 0, 14, ERROR);
	test2yday(2020, 13, 14, ERROR);
	test2yday(2020, 6, 0, ERROR);
	test2yday(2020, 2, 30, ERROR);
	test2yday(2021, 2, 39, ERROR);

	test2month(2020, 115, 4, 24);
	test2month(2020, 366, 12, 31);
	test2month(2021, 365, 12, 31);

	test2month(2020, 367, ERROR, ERROR);
	test2month(2021, 366, ERROR, ERROR);
	test2month(2022, 0, ERROR, ERROR);
	return 0;
}

void test2yday(int year, int month, int day, int expected)
{
	int actual = day_of_year(year, month, day);

	if (actual == expected) {
		printf("ok: %d-%d-%d  -> %d\n", year, month, day, actual);
	} else {
		printf("FAIL: %d-%d-%d  -> %d (expected %d)\n", year, month,
		       day, actual, expected);
	}
}

void test2month(int year, int day, int exmonth, int exday)
{
	int actmonth, actday;
	actmonth = actday = 0;
	month_day(year, day, &actmonth, &actday);

	if (actmonth == exmonth && actday == exday) {
		printf("ok: %d %d  -> %d:%d:%d\n", year, day, year, actmonth,
		       actday);
	} else {
		printf("FAIL: %d %d  -> %d:%d:%d (expected %d %d)\n", year, day,
		       year, actmonth, actday, exmonth, exday);
	}
}

int day_of_year(int year, int month, int day)
{
	int i, leap;

	leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
	if (month < 1 || 12 < month) {
		printf("error: Invalid month: %d\n", month);
		return ERROR;
	}
	if (day < 1 || daytab[leap][month] < day) {
		printf("error: Invalid day: %d\n", day);
		return ERROR;
	}
	for (i = 1; i < month; i++)
		day += daytab[leap][i];
	return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;

	leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
	if (yearday < 1 || (365 + leap) < yearday) {
		printf("error: Invalid day of year: %d\n", yearday);
		*pmonth = *pday = ERROR;
		return;
	}
	for (i = 1; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;
}
