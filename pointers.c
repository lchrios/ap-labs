#include <stdio.h>


int main(){
    
    int x = 1, y = 2, z[10];
    int *ip;         /* ip is a pointer to int */
    printf("Pointer Address: %lu \n", ip);

    ip = &x;         /* ip now points to x */
    printf("Pointer Value: %d \n", *ip); 

    y = *ip;         /* y is now 1 */
    *ip = 0;         /* x is now 0 */
    ip = &z[0];      /* ip now points to z[0] */

}