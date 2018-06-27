#include "apue.h"

static volatile sig_atomic_t sigflag;
static sig_atomic_t newmask, oldmask, zeromask;

static void sig_usr(int signo) { sigflag = 1; }

void TELL_WAIT() {
  if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
    err_sys("signal (SIGUSR1) error");
  }
  if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
    err_sys("signal (SIGUSR2) error");
  }

  sigemptyset(&zeromask);
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGUSR1);
  sigaddset(&newmask, SIGUSR2);

  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
    err_sys("SIG_BLOCK error");
  }
}

void TELL_PARENT(pid_t pid) { kill(pid, SIGUSR2); }

void WAIT_PARENT() {
  while (sigflag == 0) {
    sigsuspend(&zeromask);
  }
  sigflag = 0;

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
    err_sys("SIG_SETMASK error");
  }
}

void TELL_CHILD(pid_t pid) { kill(pid, SIGUSR1); }

void WAIT_CHILD() {
  while (sigflag == 0) {
    sigsuspend(&zeromask);
  }
  sigflag = 0;

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
    err_sys("SIG_SETMASK error");
  }
}

void writenum(int number) {
  FILE *file = fopen("p6.txt", "w");
  fprintf(file, "%d\n", number);
  fclose(file);
}

int main() {
  FILE *file = fopen("p6.txt", "w");
  fprintf(file, "0\n");
  fclose(file);

  int cur = 1;
  pid_t pid;

  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    for (; cur < 1000; cur += 2) {
      TELL_WAIT();
      writenum(cur);
      printf("子进程输出了 %d\n", cur);
      TELL_PARENT(getppid());
      WAIT_PARENT();
    }
  } else {
    for (cur += 1; cur < 1000; cur += 2) {
      TELL_WAIT();
      WAIT_CHILD();
      writenum(cur);
      printf("父进程输出了 %d\n", cur);
      TELL_CHILD(pid);
    }
  }
}