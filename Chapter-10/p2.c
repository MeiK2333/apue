#include <signal.h>
#include <stdio.h>
#include <string.h>

#define s2s(signo, str, signal)                \
  {                                            \
    if (signo == signal) strcpy(str, #signal); \
  }

void sig2str(int signo, char *str) {
  strcpy(str, "UNKNOWN");
  s2s(signo, str, SIGKILL);
  s2s(signo, str, SIGHUP);
  s2s(signo, str, SIGINT);
  s2s(signo, str, SIGQUIT);
  s2s(signo, str, SIGILL);
  s2s(signo, str, SIGTRAP);
  s2s(signo, str, SIGABRT);
  s2s(signo, str, SIGIOT);
  s2s(signo, str, SIGBUS);
  s2s(signo, str, SIGFPE);
  s2s(signo, str, SIGKILL);
  s2s(signo, str, SIGUSR1);
  s2s(signo, str, SIGSEGV);
  s2s(signo, str, SIGUSR2);
  s2s(signo, str, SIGPIPE);
  s2s(signo, str, SIGALRM);
  s2s(signo, str, SIGTERM);
  s2s(signo, str, SIGSTKFLT);
  s2s(signo, str, SIGCHLD);
  s2s(signo, str, SIGCONT);
  s2s(signo, str, SIGSTOP);
  s2s(signo, str, SIGTSTP);
  s2s(signo, str, SIGTTIN);
  s2s(signo, str, SIGTTOU);
  s2s(signo, str, SIGURG);
  s2s(signo, str, SIGXCPU);
  s2s(signo, str, SIGXFSZ);
  s2s(signo, str, SIGVTALRM);
  s2s(signo, str, SIGPROF);
  s2s(signo, str, SIGWINCH);
  s2s(signo, str, SIGIO);
  s2s(signo, str, SIGPWR);
  s2s(signo, str, SIGSYS);
  s2s(signo, str, SIGRTMIN);
}

int main() {
  int n;
  char str[1024];
  while (1) {
    scanf("%d", &n);
    sig2str(n, str);
    printf("%s\n", str);
  }
}