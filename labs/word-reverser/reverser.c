#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void reverse(char *x, int begin, int end) {
    char c;

    if (begin >= end)
        return;

    c          = *(x+begin);
    *(x+begin) = *(x+end);
    *(x+end)   = c;

    reverse(x, ++begin, --end);
}

int main(){
    // Place your magic here
    char c, str[100];
    int i = 0;

    while(true){

        i = 0;
                
        printf("Enter a word to reverse: ");

        c = getchar();
        while (c != '\n') {
            
            if (c == EOF) {printf("\n");return 0;}
            str[i] = c;
            i++;
            c = getchar();

        }
        str[i] = '\0';

        reverse(str, 0, strlen(str) - 1);
        printf("Reversed word: %s\n", str);

    }
    


    return 0;
}