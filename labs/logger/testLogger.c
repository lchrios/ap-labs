#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"

int main(int argc, char **argv) {
    printf("%d %s\n", argc, argv);

    infof("%d", 1, argc);
    warnf("%d", 1, argc);
    errorf("%d", 1, argc);
    panicf("%d", 1, argc);
    
    return 0;
}
