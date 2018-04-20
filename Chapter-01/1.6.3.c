#include "apue.h"

#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char buf[MAXLINE];  // apue.h 中定义 #define	MAXLINE	4096
    pid_t pid;
    int status;

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {
            execlp(buf, buf, (char *) 0);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");
        printf("%% ");
    }
    exit(0);
}