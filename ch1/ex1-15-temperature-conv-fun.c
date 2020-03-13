/* Exercise 1-15.

Rewrite the temperature conversion program of Section 1.2 to use a function
for conversion.

*/

#include <stdio.h>

#define   LOWER  0       /* lower limit of temperature */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

float fahr2celsius(float fahar);

/* print Fahrenhiet-Celsius table
    for fahr - 0, 20, ..., 300 */
int main()
{
    int fahr;

    printf("\nTemperature conversion from degress\n");
    printf("Fahrenheit to degrees Celsius\n\n");
    printf("   °F     °C\n");
    printf("=====  =====\n");

    for(fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
        printf("%4d %6.1f\n", fahr, fahr2celsius(fahr));

    printf("\n");

    return 0;
}

/* convert Fahrenheit temperature to Celsius */
float fahr2celsius(float fahr){
    return (5.0/9.0)*(fahr-32);
}

/* Sample Output

Temperature conversion from degress
Fahrenheit to degrees Celsius

   °F     °C
=====  =====
   0  -17.8
  20   -6.7
  40    4.4
  60   15.6
  80   26.7
 100   37.8
 120   48.9
 140   60.0
 160   71.1
 180   82.2
 200   93.3
 220  104.4
 240  115.6
 260  126.7
 280  137.8
 300  148.9

*/
