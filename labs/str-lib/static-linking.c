#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strlib.h"

int main(int argc, char *argv[]) {

    if (strncmp(argv[1], "-add", 4) == 0 && !(argv[2] == NULL || argv[3] == NULL)){

        int initLen = mystrlen(argv[2]);
        char *nStr = mystradd(argv[2], argv[3]);
        int nLen = mystrlen(nStr);
        
        printf("=== ADDING STRINGS ===");
        printf("Initial Length: %d\nNew String: %s\nNew Length: %d\n", initLen, nStr, nLen);

        free(nStr);
    
    }
    else if(strncmp(argv[1], "-find", 4) == 0 && !(argv[2] == NULL || argv[3] == NULL)){

        int wasFound = mystrfind(argv[2], argv[3]);
        
        if (wasFound >= 0){
            printf("Substring: %s\nFound at: %d\n", argv[3], wasFound);
        }
        else{
            printf("Substring was not found");
        }
    
    }
    else{

        printf("Arguments are not correctly inputed.\n");
    
    }
    return 0;
}
