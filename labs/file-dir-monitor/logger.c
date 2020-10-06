#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdbool.h>
#include "logger.h"

#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		10
#define RED		    1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

/*
    LOG_CON | Write directly to system console if there is an error while sending to system logger.
    LOG_NDELAY | Open the connection immediately 
*/

void textcolor(int attr, int fg, int bg);

// default - syslog 
bool isSyslog = true;

int initLogger(char *logType) {

    // stdout selected
    if (strcmp(logType, "") == 0 || strcmp(logType, "stdout") == 0) { 
        
        isSyslog = false; 
        
    }

    // syslog selected
    else if (strcmp(logType, "syslog") == 0){ 
        
        setlogmask (LOG_UPTO (LOG_DEBUG));
        isSyslog = true; 
        
    }

    // wrong parameter typed
    else {

        perror("Error: at logging stream selection\nPlease use one of these options: (stdout/syslog)");
        return 1;
    
    }

    // logTpye is valid
    printf("Logger initialized correctly...\n\tStreaming messages through: %s\n\n", isSyslog ? "syslog" : "stdout");

    return 0;

}


int infof(const char *format, ...) {
    
    // getting parameters 
    va_list ap;
    va_start(ap, format);

    // change console style
    textcolor(BRIGHT, CYAN, BLACK);
    
    // Logging data
    if(isSyslog){

        openlog("INFO", LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
        vsyslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), format, ap);
		closelog();

    } else {

        vfprintf(stdout, format, ap);
    
    }

    va_end(ap);

    if (isSyslog == 0) {

        // clean stdout console
		fprintf(stdout, "\n");
		fflush(stdout);
	
    }

    // reset log to normal style
    textcolor(RESET, WHITE, BLACK);

    return 0;

}


int warnf(const char *format, ...) {

    // getting parameters
    va_list ap;
    va_start(ap,format);

    // change console stlye
    textcolor(BRIGHT, YELLOW, BLACK);

    // Logging data
    if (isSyslog){

        //openlog("WARN", LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
		vsyslog(LOG_MAKEPRI(LOG_USER, LOG_WARNING), format, ap);
		//closelog();
    
    } else {
       
        vfprintf(stdout, format, ap);
	
    }

    va_end(ap);

    if (isSyslog == 0) {
        
		// clean stdout console
        fprintf(stdout, "\n");
		fflush(stdout);
	
    }

    // reset log to normal style
    textcolor(RESET, WHITE, BLACK);

    return 0;

}


int errorf(const char *format, ...) {

    // getting parameters
    va_list ap;
    va_start(ap,format);

    // change console stlye
    textcolor(BRIGHT, RED, BLACK);

    // Logging data
    if (isSyslog){

        openlog("ERROR", LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
		vsyslog(LOG_MAKEPRI(LOG_USER, LOG_ERR), format, ap);
		closelog();
    
    } else {

        vfprintf(stdout, format, ap);
	
    }

    va_end(ap);

    if (isSyslog == 0) {

	    // clean stdout console	
        fprintf(stdout, "\n");
		fflush(stdout);
	
    }

    // reset log to normal style
    textcolor(RESET, WHITE, BLACK);

    return 0;

}


int panicf(const char *format, ...) {
    
    // getting parameters
    va_list ap;
    va_start(ap,format);

    // change console stlye
    textcolor(BRIGHT, MAGENTA, BLACK);

    // Logging data
    if (isSyslog){

        openlog("PANIC", LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
		vsyslog(LOG_MAKEPRI(LOG_USER, LOG_EMERG), format, ap);
		closelog();
    
    } else {

        vfprintf(stdout, format, ap);

	}
    
    va_end(ap);

    if (isSyslog == 0) {

        // clean stdout console
		fprintf(stdout, "\n");
		fflush(stdout);
	
    }

    // reset log to normal style
    textcolor(RESET, WHITE, BLACK);
    
    return 0;
}


void textcolor(int attr, int fg, int bg) {

    char command[13];

	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);

}