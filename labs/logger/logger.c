#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		    1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void textcolor (int attr, int fg, int bg) {
        
        char command[13];

        sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
        printf("%s", command);

}

int infof(const char *format, int count, ...) {
    va_list values;

    va_start(values, count);

    textcolor(BRIGHT, CYAN, BLACK);
    printf("[INFO] ");
    textcolor(RESET, CYAN, BLACK);

    vprintf(format, values);

}

int warnf(const char *format, int count, ...) {
    va_list values;

    va_start(values, count);

    textcolor(BRIGHT, MAGENTA, BLACK);
    printf("[WARN] ");
    textcolor(RESET, MAGENTA, BLACK);

    vprintf(format, values);

}


int errorf(const char *format, int count, ...) {
    va_list values;

    va_start(values, count);

    textcolor(BRIGHT, RED, BLACK);
    printf("[ERROR] ");
    textcolor(RESET, RED, BLACK);

    vprintf(format, values);

}


int panicf(const char *format, int count, ...) {
    va_list values;

    va_start(values, count);

    textcolor(BRIGHT, YELLOW, BLACK);
    printf("[PANIC!] ");
    textcolor(RESET, YELLOW, BLACK);

    vprintf(format, values);

}
