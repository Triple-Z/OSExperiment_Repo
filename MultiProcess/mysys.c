/**
 * Author: TripleZ
 * Date: 6/17/2018
 * 
 * Simplest implemention for "system" function.
 * 
 **/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int mysys(char* command) {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command, NULL);
    } else {
        wait(NULL);
    }
}

int main (int argc, char* argv[]) {
    printf("-----------------------------------------\n");
    mysys("echo Hello world");
    printf("-----------------------------------------\n");
    mysys("ls /");
    printf("-----------------------------------------\n");
}

