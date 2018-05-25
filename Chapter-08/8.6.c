#include <sys/wait.h>
#include "apue.h"

void pr_exit(int status) {
  if (WIFEXITED(status)) {
    printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
           WCOREDUMP(status) ? "  (core file generated)" : "");
#else
           "");
#endif
  } else if (WIFSTOPPED(status)) {
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
  }
}

int main() {
  pid_t pid;
  int status;

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    exit(7);
  }

  if (wait(&status) != pid) {
    err_sys("wait error");
  }
  pr_exit(status);

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    abort();
  }

  if (wait(&status) != pid) {
    err_sys("wait error");
  }
  pr_exit(status);

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    status /= 0;
  }

  if (wait(&status) != pid) {
    err_sys("wait error");
  }
  pr_exit(status);

  exit(0);
}