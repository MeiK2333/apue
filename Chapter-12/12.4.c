#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "apue.h"

int makethread(void *(*fn)(void *), void *arg) {
  int err;
  pthread_t tid;
  pthread_attr_t attr;

  err = pthread_attr_init(&attr);
  if (err != 0) return (err);
  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (err == 0) err = pthread_create(&tid, &attr, fn, arg);
  pthread_attr_destroy(&attr);
  return (err);
}

struct to_info {
  void (*to_fn)(void *);
  void *to_arg;
  struct timespec to_wait;
};

#define SECTONSEC 1000000000

#if !defined(CLICK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM) nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000

void clock_gettime(int id, struct timespec *tsp) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  tsp->tv_sec = tv.tv_sec;
  tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

void *timeout_helper(void *arg) {
  struct to_info *tip;

  tip = (struct to_info *)arg;
  clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);
  (*tip->to_fn)(tip->to_arg);
  free(arg);
  return (0);
}

void timeout(const struct timespec *when, void (*func)(void *), void *arg) {
  struct timespec now;
  struct to_info *tip;
  int err;

  clock_gettime(CLOCK_REALTIME, &now);
  if ((when->tv_sec > now.tv_sec) ||
      (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
    tip = malloc(sizeof(struct to_info));
    if (tip != NULL) {
      tip->to_fn = func;
      tip->to_arg = arg;
      tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
      if (when->tv_nsec >= now.tv_nsec) {
        tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
      } else {
        tip->to_wait.tv_sec--;
        tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
      }
      err = makethread(timeout_helper, (void *)tip);
      if (err == 0) {
        return;
      } else {
        free(tip);
      }
    }
  }
  (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void retry(void *arg) {
  pthread_mutex_lock(&mutex);

  pthread_mutex_unlock(&mutex);
}

int main(void) {
  int err, condition, arg;
  struct timespec when;

  if ((err = pthread_mutexattr_init(&attr)) != 0) {
    err_exit(err, "pthread_mutexattr_init failed");
  }
  if ((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP)) !=
      0) {
    err_exit(err, "can't set recursive type");
  }
  if ((err = pthread_mutex_init(&mutex, &attr)) != 0) {
    err_exit(err, "can't create recursive mutex");
  }
  pthread_mutex_lock(&mutex);

  if (condition) {
    clock_gettime(CLOCK_REALTIME, &when);
    when.tv_sec += 10;
    timeout(&when, retry, (void *)((unsigned long)arg));
  }
  pthread_mutex_unlock(&mutex);
  exit(0);
}