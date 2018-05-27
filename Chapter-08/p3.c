#include <signal.h>
#include <sys/wait.h>
#include "apue.h"

void pr_exit_2(siginfo_t *infop) {
  printf("signo: %d\n", infop->si_signo);
  printf("code: %d\n", infop->si_code);
}

int main() {
  pid_t pid;
  int status;
  siginfo_t infop;

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    exit(7);
  }

  if (waitid(P_ALL, 0, &infop, WEXITED) != 0) {
    err_sys("waitid error");
  }
  pr_exit_2(&infop);

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    abort();
  }

  if (waitid(P_ALL, 0, &infop, WEXITED) != 0) {
    err_sys("waitid error");
  }
  pr_exit_2(&infop);

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    status /= 0;
  }

  if (waitid(P_ALL, 0, &infop, WEXITED) != 0) {
    err_sys("waitid error");
  }
  pr_exit_2(&infop);

  exit(0);
}