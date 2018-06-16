/**
 * Author: TripleZ
 * Date: 6/16/2018
 * 
 * Simplest implemention for "cat" command.
 * 
 **/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 255

int main (int argc, char* argv[]) {
    int fd;
    char buffer[BUFFER_SIZE];
    int status;
    
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("Cannot open this file !\n\a");
    } else {        
        while (status = read(fd, buffer, BUFFER_SIZE)) {

            if (status == BUFFER_SIZE) {
                printf("%s", buffer);
            } else {
                buffer[status] = '\0';
                printf("%s", buffer);
            }
        }
    }

    close(fd);

    return 0;
}