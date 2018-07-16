#include <fcntl.h>
#include <sys/mman.h>
#include "apue.h"

#define NLOOPS 1000
#define SIZE sizeof(long)

static int update(long *ptr) { return ((*ptr)++); }

int main() {
  int i, counter;
  pid_t pid;
  void *area;

  if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1,
                   0)) == MAP_FAILED) {
    err_sys("mmap error");
  }

  TELL_WAIT();
  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid > 0) {
    for (i = 0; i < NLOOPS; i += 2) {
      if ((counter = update((long *)area)) != i) {
        err_quit("parent: expected %d, got %d", i, counter);
      }
      printf("update %d from parent\n", i);

      TELL_CHILD(pid);
      WAIT_CHILD();
    }
  } else {
    for (i = 1; i < NLOOPS + 1; i += 2) {
      WAIT_PARENT();

      if ((counter = update((long *)area)) != i) {
        err_quit("child:expected %d, got %d", i, counter);
      }
      printf("update %d from child\n", i);

      TELL_PARENT(getppid());
    }
  }
  exit(0);
}
