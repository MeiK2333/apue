#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "apue.h"

#define NLOOPS 1000
#define SIZE sizeof(long)

static int update(long *ptr) { return ((*ptr)++); }

int main() {
    int i, counter;
    pid_t pid;
    void *area;

    sem_t *st1, *st2;

    st1 = sem_open("/test1", O_CREAT, 0600, 0);
    st2 = sem_open("/test2", O_CREAT, 0600, 0);
    sem_unlink("/test1");
    sem_unlink("/test2");

    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1,
                     0)) == MAP_FAILED) {
        err_sys("mmap error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i) {
                err_quit("parent: expected %d, got %d", i, counter);
            }
            printf("update %d from parent\n", i);

            sem_post(st2);
            sem_wait(st1);
        }
    } else {
        for (i = 1; i < NLOOPS + 1; i += 2) {
            sem_wait(st2);

            if ((counter = update((long *)area)) != i) {
                err_quit("child:expected %d, got %d", i, counter);
            }
            printf("update %d from child\n", i);

            sem_post(st1);
        }
    }
    exit(0);
}
