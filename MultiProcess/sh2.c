/**
 * Author: TripleZ
 * Date: 6/17/2018
 * 
 * Simplest implemention for shell - II.
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int stdIn, stdOut;

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

    dup2(stdIn, 0);
    dup2(stdOut, 1);
}

int parseline(char *buf, char **argv){ 
    if (strcmp(buf, "\n") == 0) {
        return 0;
    }

    char *p = NULL;
    char *filename = NULL;
    int argc = 0;
    p = strtok(buf, " \n");  // Manipulate in original string directly.
    while (p != NULL) {
        
        if (p[0] == '>') { // Redirect output to file
            // if p[1] is not space, the last is the filename.
            if(p[1] != ' ' && p[1] != '\n' && p[1] != '\0') {
                filename = p+1;
            } else {
                // printf("\n----strtok---\n");
                filename = strtok(NULL, "> \n");
            }

            // printf("filename = %s\n", filename);

            if (filename != NULL) {

                // Redirect standard output to a specific file
                int fd;
                fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);
                if (fd < 0) {
                    printf("TripleZsh: File open error.\n");
                    return -1;
                }

                dup2(fd, 1);  // Redirect standard output to file
                break;

            } else {
                printf("TripleZsh: Missing redirect file.\n");
            }
        } else {
            argv[argc++] = p;
            p = strtok(NULL, " \n");
        }

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

    stdIn = dup(0);
    stdOut = dup(1);
    
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


