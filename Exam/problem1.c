#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <wait.h>

int main (int argc, char *argv[]) {
	pid_t pid;
	int fd[2];
	char buf[50] = {0};
	pipe(fd);  // pipe should be created at first !

	pid = fork();
	if (pid == 0) {
		// child process
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execlp("echo", "echo", "hello world", NULL);
		exit(0);
	} else {
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
	}

	read(0, buf, sizeof(buf));
	printf("echo content: %s\n", buf);
	return 0;

}
