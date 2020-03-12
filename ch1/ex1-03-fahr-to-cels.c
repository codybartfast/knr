#include <stdio.h>

/* print Fahrenhiet-Celsius table
    for fahr - 0, 20, ..., 300 */
int main()
{
    float fahr, celsius;
    int lower, upper, step;

    lower = 0;      /* lower limit of temperature */
    upper = 300;    /* upper limit */
    step = 20;      /* step size */

    printf("Temperature conversion from degress\n");
    printf("Fahrenheit to degrees Celsius\n\n");
    printf("   °F     °C\n");
    printf("=====  =====\n");

    fahr = lower;
    while (fahr <= upper) {
        celsius = (5.0/9.0) * (fahr-32.0);
        printf("%5.0f %6.1f\n", fahr, celsius);
        fahr = fahr + step;
    }
}
