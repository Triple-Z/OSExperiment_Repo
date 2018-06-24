#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {
    pid_t pid_a = -1, pid_b = -1;
    int fd_a[2], fd_b[2];

    pipe(fd_a);
    pipe(fd_b);

    pid_a = fork();

    if (pid_a == 0) {
        int i, sum = 0;
        for (i = 1; i <= 50; i++) {
            sum += i;
        }
        
        write(fd_a[1], &sum, sizeof(sum));
        exit(0);
    }
    
    pid_b = fork();

    if (pid_b == 0) {
        int i, sum = 0;
        for (i = 51; i <= 100; i++) {
            sum += i;
        }
        
        write(fd_b[1], &sum, sizeof(sum));
        exit(0);
    }

    int sum = 0, buf;
    read(fd_a[0], &buf, sizeof(buf));
    sum += buf;
    read(fd_b[0], &buf, sizeof(buf));
    sum += buf;

    printf("The result from 1 add to 100 is: %d\n", sum);

    return 0;

}
