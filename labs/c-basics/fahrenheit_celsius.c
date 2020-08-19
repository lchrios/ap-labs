#include <stdio.h>

//#define   LOWER  0       /* lower limit of table */
//#define   UPPER  300     /* upper limit */
//#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    int fahr;
    int LOWER;
    sscanf(argv[1],"%d", &LOWER);
    int UPPER;
    sscanf(argv[2],"%d", &UPPER);
    int STEP;
    sscanf(argv[3],"%d", &STEP);
    for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
    printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    return 0;
}
