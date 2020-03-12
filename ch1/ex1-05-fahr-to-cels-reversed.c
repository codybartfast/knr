#include <stdio.h>

/* print Fahrenhiet-Celsius table
    for fahr - 300, 280, ..., 0 */
int main()
{
    int fahr;

    printf("Temperature conversion from degress\n");
    printf("Fahrenheit to degrees Celsius\n\n");
    printf("  °F     °C\n");
    printf("====  =====\n");

    for(fahr = 300; fahr >= 0; fahr = fahr - 20)
        printf("%4d %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    printf("\n");
}
