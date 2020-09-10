#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* month_day function's prototype*/

bool isBisiesto(int year) {
    if (year % 400 == 0) { return true; } 
    else if (year % 100 == 0) { return false; } 
    else if (year % 4 == 0) { return true; } 
    else { return false; }
}

void month_day(int *year, int *yearday, int *pmonth, int *pday) {
    
    const char *monthName[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int monthLen[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    monthLen[1] += isBisiesto(*year) ? 1 : 0;

    for (int i = 0; i < *yearday; i++) {
        // Same month
        if ((*pday + 1) < monthLen[*pmonth]) {
            *pday += 1;
        } 
        // Month has changed
        else {
            *pday = 0;
            *pmonth += 1;
            if (*pmonth > 11) {
                *year += 1;
                *pmonth = 0;
            }
        }
    }

    printf("%s %2d, %4d\n", monthName[*pmonth], *pday, *year);  

}



int main(int argc, char *argv[]) {

    int pmonth=0; 
    int pday=0;

    char *y = argv[1];
    char *yd = argv[2];
    int year = atoi(y), yearday = atoi(yd);

    month_day(&year, &yearday, &pmonth, &pday);

    
    
    
    return 0;
}
