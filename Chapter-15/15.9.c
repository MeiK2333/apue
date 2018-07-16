#include <sys/shm.h>
#include "apue.h"

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600

char array[ARRAY_SIZE];

int main() {
  int shmid;
  char *ptr, *shmptr;

  printf("array[] from %p to %p\n", (void *)&array[0],
         (void *)&array[ARRAY_SIZE]);
  printf("stack around %p\n", (void *)&shmid);

  if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
    err_sys("malloc error");
  }
  printf("malloced from %p to %p\n", (void *)ptr, (void *)ptr + MALLOC_SIZE);

  if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
    err_sys("shmget error");
  }
  if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1) {
    err_sys("shmat error");
  }
  printf("shared memory attached from %p to %p\n", (void *)shmptr,
         (void *)shmptr + SHM_SIZE);

  if (shmctl(shmid, IPC_RMID, 0) < 0) {
    err_sys("shmctl error");
  }

  exit(0);
}