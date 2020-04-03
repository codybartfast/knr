/*
 * Exercise 3-1
 *
 * Our binary search makes two tests inside the loop, when one would suffice
 * (at the price of more tests outside).  Write a version with only one test
 * inside the loop and measure the difference in run-time.
 */

#include <stdio.h>

#define SIZE 1 << 25

int binsearch(int x, int v[], int n);

int values[SIZE];

int main(void)
{
	int i, n = SIZE;

	for (i = 0; i < n; i++)
		values[i] = i;
	/* search for every item */
	for (i = 0; i < n; i++)
		binsearch(i, values, n);
	return 0;
}

int binsearch(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low < high) {
		mid = (low + high) / 2;
		if (x > v[mid])
			low = mid + 1;
		else
			high = mid;
	}
	return (x == v[low]) ? low : -1;
}

/*
With arrays containing 33M-134M items and then searching for each item I
found a measurable difference in run-time with the modified version taking
approx. 4% longer.  The extra steps to terminate in the modified version
being more expensive than the cost of the extra test in the original
version.

Size	Original	Modified	Diff
====    ========        ========        ====
2^25	 4.042s		 4.188s		3.6%
2^26	 8.365s		 8.551s		2.2%
2^27	16.854s		17.818s		5.7%
(each test run 3 times)
*/
