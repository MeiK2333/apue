#include "apue.h"

static void sig_quit(int);

int main() {
  sigset_t newmask, oldmask, pendmask;

  if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
    err_sys("can't catch SIGQUIT");
  }

  sigemptyset(&newmask);         // 初始化信号集
  sigaddset(&newmask, SIGQUIT);  // 将 SIGQUIT 加入信号集中
  // 为进程设置新的屏蔽字并将旧的赋值给 oldmask
  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
    err_sys("SIG_BLOCK error");
  }

  sleep(5);

  if (sigpending(&pendmask) < 0) {  // 获得当前未决信号
    err_sys("sigpending error");
  }
  if (sigismember(&pendmask, SIGQUIT)) {  // 判断指定信号是否存在
    printf("\nSIGQUIT pending\n");
  }

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {  // 恢复之前的信号集
    err_sys("SIG_SETMASK error");
  }
  printf("SIGQUIT unblocked\n");

  sleep(5);
  exit(0);
}

static void sig_quit(int signo) {
  printf("caught SIGQUIT\n");
  if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
    err_sys("can't reset SIGQUIT");
  }
}