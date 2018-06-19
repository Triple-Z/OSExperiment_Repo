/**
 * Author: TripleZ
 * Date: 6/17/2018
 * 
 * Simplest implemention for shell - I.
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define BUFFER_SIZE 1024


void eval(char *cmd) {
    char *argv[BUFFER_SIZE] = {NULL};
    char buf[BUFFER_SIZE] = {0};

    int j;
    for (j = 0; j < BUFFER_SIZE; j++) {
        argv[j] = NULL;
    }

    strcpy(buf, cmd);

    parseline(buf, argv);
    if (argv[0] == NULL) {
        return ;
    }

    // char** test = NULL;
    // printf("\n=====parseline========\n");
    // for (test = argv; *test != NULL; test++) {
    //     printf("%s\n", *test);
    // }
    // if (*test == NULL) {
    //     printf("NULL\n");
    // }
    // printf("\n==================\n");

    if (buildinCommand(argv)) return;
    
    pid_t pid = fork();
    if (pid == 0) {
        // char** test = NULL;
        // printf("\n=======childProcess=======\n");
        // for (test = argv; *test != NULL; test++) {
        //     printf("%s\n", *test);
        // }
        // if (*test == NULL) {
        //     printf("NULL\n");
        // }
        // printf("\n==================\n");
        if (execvp(argv[0], argv) < 0) {
            // Execute failed
            printf("TripleZsh: %s: command not found...\n", argv[0]);
            exit(0);
        }
    } else {
        wait(NULL);
    }
}

int parseline(char *buf, char **argv){ 
    if (strcmp(buf, "\n") == 0) {
        return 0;
    }

    char *p = NULL;
    int argc = 0;
    p = strtok(buf, " \n");  // Manipulate in original string directly.
    while (p != NULL) {
        argv[argc++] = p;
        p = strtok(NULL, " \n");
    }
    argv[argc] = NULL;

    return 0;
}

int buildinCommand(char **argv){
    

    if(strcmp(argv[0],"exit") == 0){
        exit(0);
    }
    if(strcmp(argv[0],"cd") == 0){
        if(chdir(argv[1])){
            printf("TripleZsh: cd: no such file or directory: %s\n",argv[1]);
        }
        return 1;
    }
    if(strcmp(argv[0],"pwd") == 0){
        char buf[BUFFER_SIZE];
        printf("%s\n",getcwd(buf,sizeof(buf)));
        return 1;
    }
    return 0;//not a buildin_command
}


int main (int argc, char *argv[]) {
    char buf[BUFFER_SIZE] = {0};
    int n;
    
    while(1) {
        printf("TripleZsh> ");
        fflush(stdout);
        
        int i;
        for (i = 0; i < BUFFER_SIZE; i++) {
            buf[i] = 0;
        }

        if ((n = read(0, buf, BUFFER_SIZE)) < 0) {
            printf("There has something wrong in input .\n");
        }

        eval(buf);

    }

    return 0;
}


