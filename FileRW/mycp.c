/**
 * Author: TripleZ
 * Date: 6/17/2018
 * 
 * Simplest implemention for "cp" command.
 * 
 **/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 255

int main(int argc, char* argv[]) {
    const char* source = argv[1];
    const char* dest = argv[2];
    mode_t mode = 0777;
    char buf[BUFFER_SIZE];
    int fs, fd;
    int rCount, wCount;

    fs = open(source, O_RDONLY);
    
    if (!fs) {
        printf("Cannot open source file");
    }

    fd = open(dest, O_RDWR | O_TRUNC | O_CREAT, mode);
    if (!fd) {
        printf("Cannot open/create destination file");
    }

    while(rCount = read(fs, buf, BUFFER_SIZE)) {
        if (rCount == BUFFER_SIZE) {
            wCount = write(fd, buf, BUFFER_SIZE);
        } else {
            
            buf[rCount] = '\0';
            wCount = write(fd, buf, rCount);
        }
    }

    close(fs);
    close(fd);

    return 0;
}