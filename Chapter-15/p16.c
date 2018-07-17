#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include "apue.h"

#define NLOOPS 1000
#define SIZE sizeof(long)

static int update(long *ptr) { return ((*ptr)++); }

int main() {
    int i, counter;
    pid_t pid;
    void *area;
    int semid;
    struct sembuf sbf[1];
    semid = semget(0, 1, IPC_CREAT | 0666);

    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1,
                     0)) == MAP_FAILED) {
        err_sys("mmap error");
    }

    // sbf[0].sem_op = 1;
    // semop(semid, sbf, 1);
    // TELL_WAIT();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i) {
                err_quit("parent: expected %d, got %d", i, counter);
            }
            printf("update %d from parent\n", i);

            sbf[0].sem_op = i + 1;
            semop(semid, sbf, 1);
            sbf[0].sem_op = -i - 2;
            semop(semid, sbf, 1);
            // TELL_CHILD(pid);
            // WAIT_CHILD();
        }
    } else {
        for (i = 1; i < NLOOPS + 1; i += 2) {
            sbf[0].sem_op = -i;
            semop(semid, sbf, 1);
            // WAIT_PARENT();

            if ((counter = update((long *)area)) != i) {
                err_quit("child: expected %d, got %d", i, counter);
            }
            printf("update %d from child\n", i);

            sbf[0].sem_op = i + 1;
            semop(semid, sbf, 1);
            // TELL_PARENT(getppid());
        }
    }
    exit(0);
}
