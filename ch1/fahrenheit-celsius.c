#include <stdio.h>

#define   LOWER  0       /* lower limit of temperature */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenhiet-Celsius table
    for fahr - 0, 20, ..., 300 */
int main()
{
    int fahr;

    printf("Temperature conversion from degress\n");
    printf("Fahrenheit to degrees Celsius\n\n");
    printf("   °F     °C\n");
    printf("=====  =====\n");

    for(fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
        printf("%4d %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    printf("\n");
}