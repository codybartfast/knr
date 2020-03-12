#include <stdio.h>

/* print Celsius-Fahrenhiet table
    for celsius - -20, -10, ..., 150 */
int main()
{
    float celsius, fahr;
    int lower, upper, step;

    lower = -20;    /* lower limit of temperature */
    upper = 150;    /* upper limit */
    step = 10;      /* step size */

    printf("Temperature conversion from degress\n");
    printf("Celsius to degrees Fahrenheit\n\n");
    printf("   °C     °F\n");
    printf("=====  =====\n");

    celsius = lower;
    while (celsius <= upper) {
        fahr = (celsius * 9.0) / 5.0 + 32.0;
        printf("%5.0f %6.1f\n", celsius, fahr);
        celsius = celsius + step;
    }
}
