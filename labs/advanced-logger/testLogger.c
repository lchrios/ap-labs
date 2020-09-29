#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

int main(int argc, char **argv){
	int cs;


	if(argc > 1){
		cs = atoi(argv[1]);
		//printf("%d\n", cs);
	} 
	// invalid input of data
	else {
		cs = -1;
		//printf("%d\n", cs);
	}

    switch(cs) {
		case 1:
			// default logging
			infof("INFO Message %d", 1);
			warnf("WARN Message %d", 2);
			errorf("ERROR Message %d", 2);
			break;

		case 2:
			// stdout logging
			initLogger("stdout");
			infof("INFO Message %d", 1);
			warnf("WARN Message %d", 2);
			errorf("ERROR Message %d", 2);
			break;
		
		case 3:
			// syslog logging
			initLogger("syslog");
			infof("INFO Message %d", 1);
			warnf("WARN Message %d", 2);
			errorf("ERROR Message %d", 2);
			break;
		
		default:
			initLogger("stdout");
			errorf("Invalid test case");
			break;
    }

    return 0;
}
