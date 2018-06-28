#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include "apue.h"

#define NTHR 8               /* number of threads */
#define NUMNUM 8000000L      /* number of numbers to sort */
#define TNUM (NUMNUM / NTHR) /* number to sort per thread */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void *, size_t, size_t,
                    int (*)(const void *, const void *));
#endif

/*
 * Compare two long integers (helper function for heapsort)
 */
int complong(const void *arg1, const void *arg2) {
  long l1 = *(long *)arg1;
  long l2 = *(long *)arg2;

  if (l1 == l2) {
    return 0;
  } else if (l1 < l2) {
    return -1;
  } else {
    return 1;
  }
}

/*
 * Worker thread to sort a portion of the set of numbers.
 */
void *thr_fn(void *arg) {
  long idx = (long)arg;

  // 对该进程所分配到的序列区间进行排序
  heapsort(&nums[idx], TNUM, sizeof(long), complong);
  // 标记此线程已经完成工作
  printf("线程 %ld 完成工作\n", idx / 1000000 + 1);
  pthread_barrier_wait(&b);
  printf("线程 %ld 结束\n", idx / 1000000 + 1);

  /*
   * Go off and perform more work ...
   */
  return ((void *)0);
}

/*
 * Merge the results of the individual sorted ranges.
 */
void merge() {
  long idx[NTHR];
  long i, minidx, sidx, num;

  for (i = 0; i < NTHR; i++) {
    idx[i] = i * TNUM;
  }
  /* 归并 8 段有序的序列 */
  for (sidx = 0; sidx < NUMNUM; sidx++) {
    num = LONG_MAX;
    for (i = 0; i < NTHR; i++) {
      if ((idx[i] < (i + 1) * TNUM) && (nums[idx[i]] < num)) {
        num = nums[idx[i]];
        minidx = i;
      }
    }
    snums[sidx] = nums[idx[minidx]];
    idx[minidx]++;
  }
}

int main() {
  unsigned long i;
  struct timeval start, end;
  long long startusec, endusec;
  double elapsed;
  int err;
  pthread_t tid;

  /*
   * 使用随机数创建一个待排序的序列
   */
  srandom(1);
  for (i = 0; i < NUMNUM; i++) {
    nums[i] = random();
  }

  /*
   * 创建 8 个线程来进行排序
   */
  gettimeofday(&start, NULL);
  pthread_barrier_init(&b, NULL, NTHR + 1);
  for (i = 0; i < NTHR; i++) {
    err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
    if (err != 0) {
      err_exit(err, "can't create thread");
    }
  }
  pthread_barrier_wait(&b);
  merge();
  gettimeofday(&end, NULL);

  /*
   * 输出排序后的序列
   */
  startusec = start.tv_sec * 1000000 + start.tv_usec;
  endusec = end.tv_sec * 1000000 + end.tv_usec;
  elapsed = (double)(endusec - startusec) / 1000000.0;
  printf("sort took %.4f seconds\n", elapsed);
  // for (i = 0; i < NUMNUM; i++) {
  //   printf("%ld\n", snums[i]);
  // }
  exit(0);
}