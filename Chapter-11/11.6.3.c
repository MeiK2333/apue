#include <pthread.h>
#include "apue.h"

int main(void) {
  int err;
  struct timespec tout;
  struct tm *tmp;
  char buf[64];
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

  // 首先对 lock 加锁
  pthread_mutex_lock(&lock);
  printf("mutex is locked\n");

  // 获取系统当前时间戳，精确到纳秒
  clock_gettime(CLOCK_REALTIME, &tout);
  tmp = localtime(&tout.tv_sec);
  strftime(buf, sizeof(buf), "%r", tmp);
  printf("current time is %s\n", buf);
  tout.tv_sec += 10;

  // pthread_mutex_timedlock 接受一个绝对时间作为超时时间
  err = pthread_mutex_timedlock(&lock, &tout);

  // 打印上一条语句执行完成后的时间
  clock_gettime(CLOCK_REALTIME, &tout);
  tmp = localtime(&tout.tv_sec);
  strftime(buf, sizeof(buf), "%r", tmp);
  printf("the time is now %s\n", buf);

  // 检查第二次加锁是否成功
  if (err == 0) {
    printf("mutex locked again!\n");
  } else {
    printf("can't lock mvtex again: %s\n", strerror(err));
  }
  exit(0);
}