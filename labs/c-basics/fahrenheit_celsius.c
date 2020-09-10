#include <stdio.h>

//#define   LOWER  0       /* lower limit of table */
//#define   UPPER  300     /* upper limit */
//#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    // valor de conversion
    int LOWER;

    // conversion simple
    if (argc == 2) {

        sscanf(argv[1], "%d", &LOWER);
        printf("Fahrenheit: %3d, Celcius: %4f1\n", LOWER, (5.0/9.0)*(LOWER-32));

    } 
    // conversion con rango
    else if (argc == 4) {

        int UPPER, STEP, fahr;
        sscanf(argv[1], "%d", &LOWER);
        sscanf(argv[2],"%d", &UPPER);
        sscanf(argv[3],"%d", &STEP);
        for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
            printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    } 
    // argumentos inválidos
    else {

        printf("Argumentos introducidos inválidos.\n");

    }

    return 0;
}
