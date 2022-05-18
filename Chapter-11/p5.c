#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct s_my_pthread_barrier_t {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    unsigned int    count;
} my_pthread_barrier_t;

int my_pthread_barrier_init(my_pthread_barrier_t* barrier, unsigned int count);

int my_pthread_barrier_destroy(my_pthread_barrier_t* barrier);

int my_pthread_barrier_wait(my_pthread_barrier_t* barrier);


int my_pthread_barrier_init(my_pthread_barrier_t* barrier, unsigned int count) {
    pthread_mutex_init(&(*barrier).lock, NULL);
    pthread_cond_init(&(*barrier).cond, NULL);
    (*barrier).count = count;

    return 0;
}

int my_pthread_barrier_destroy(my_pthread_barrier_t* barrier) {
    free(barrier);

    return 0;
}

int my_pthread_barrier_wait(my_pthread_barrier_t* barrier) {
    pthread_mutex_lock(&barrier->lock);

    if (--barrier->count == 0) {
        pthread_cond_broadcast(&barrier->cond);
    }

    while (barrier->count > 0) {
        pthread_cond_wait(&barrier->cond, &barrier->lock);
    }

    pthread_mutex_unlock(&barrier->lock);

}

#define NTHR 5
my_pthread_barrier_t b;

void* thrFn(void* arg) {
    printf("thread-%ld begin\n", (long)arg);
    my_pthread_barrier_wait(&b);
    printf("thread-%ld end\n", (long)arg);
}

int main(int argc, char* argv[]) {
    pthread_t tid;
    int err;

    my_pthread_barrier_init(&b, NTHR+1);

    // create thread
    for (int i=0; i<NTHR; i++) {
        err = pthread_create(&tid, NULL, thrFn, (void*)(i*1L));
        if (err != 0) {
            perror("can't create thread");
        }
    }

    my_pthread_barrier_wait(&b);

    sleep(1);

    exit(EXIT_SUCCESS);
}
