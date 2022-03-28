#include <signal.h>
#include <sys/wait.h>
#include "apue.h"

void pr_exit_2(siginfo_t *infop) {
      if (siginfo->si_code == CLD_EXITED) {
        printf("normal termination, exit status = %d\n", siginfo->si_status);
    } else if (siginfo->si_code==CLD_KILLED || siginfo->si_code==CLD_DUMPED) {
        printf("abnormal termination, signal number = %d%s\n", siginfo->si_status,
            siginfo->si_code == CLD_DUMPED ? "(core file generated)" : "");
    } else if (siginfo->si_code == CLD_STOPPED) {
        printf("child stopped, signal number = %d\n", siginfo->si_status);
    }
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
