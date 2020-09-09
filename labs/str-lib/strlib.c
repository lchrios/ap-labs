#include "strlib.h"

int mystrlen(char *str){

    int c = 0;

    while (str[c] != '\0'){
        c++;
    }

    return c;

}

char *mystradd(char *origin, char *addition){

    int ognLen = mystrlen(origin);
    int addLen = mystrlen(addition);

    // allocate memory for the resultant string
    char *nw = (char *)malloc(ognLen+addLen);

    // write original word
    for (int i = 0; i < ognLen; i++){
        nw[i] = origin[i];
    }
    // write addition
    for (int j = 0; j < addLen; j++){
        nw[ognLen+j] = addition[j];
    }

    return nw;
    
}

int mystrfind(char *origin, char *substr){
    int substrLength = mystrlen(substr);
    int ognLen = mystrlen(origin);

    // validation for incorrect searchs
    if (substrLength > ognLen){
        return -1;
    }
    
    int found = -1;

    for(int i = 0; i < ognLen; i++){
        // search for first symbol of substring
        if (origin[i] == substr[0]){
            for(int j = 0; j < substrLength; j++){
                if (origin[i+j] == substr[j]){
                    found = i;
                }
            }
        }
    }

    return found;
}