/**
 * Author: TripleZ
 * Date: 6/16/2018
 * 
 * Simplest implemention for "echo" command.
 * 
 **/
#include <stdio.h>

int main(int argc, char * argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}
