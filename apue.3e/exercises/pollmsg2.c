#include "apue.h"
#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>

#define NQ		3		/* number of queues */
#define MAXMSZ	512		/* maximum message size */
#define KEY		0x123	/* key for first message queue */

struct mymesg {
	long		mtype;
	char		mtext[MAXMSZ+1];
};

struct threadinfo {
	int				qid;
	int				fd;
	int				len;
	pthread_mutex_t	mutex;
	pthread_cond_t	ready;
	struct mymesg	m;
};

void *
helper(void *arg)
{
	int					n;
	struct threadinfo	*tip = arg;

	for(;;) {
		memset(&tip->m, 0, sizeof(struct mymsg));
		if ((n = msgrcv(tip->qid, &tip->m, MAXMSZ, 0,
		  MSG_NOERROR)) < 0)
			err_sys("msgrcv error");
		tip->len = n;
		pthread_mutex_lock(&tip->mutex);
		if (write(tip->fd, "a", sizeof(char)) < 0)
			err_sys("write error");
		pthread_cond_wait(&tip->ready, &tip->mutex);
		pthread_mutex_unlock(&tip->mutex);
	}
}

int
main()
{
	char				c;
	int					i, n, err;
	int					fd[2];
	int					qid[NQ];
	struct pollfd		pfd[NQ];
	struct threadinfo	ti[NQ];
	pthread_t			tid[NQ];

	for (i = 0; i < NQ; i++) {
		if ((qid[i] = msgget((KEY+i), IPC_CREAT|0666)) < 0)
			err_sys("msgget error");

		printf("queue ID %d is %d\n", i, qid[i]);

		if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
			err_sys("socketpair error");
		pfd[i].fd = fd[0];
		pfd[i].events = POLLIN;
		ti[i].qid = qid[i];
		ti[i].fd = fd[1];
		if (pthread_cond_init(&ti[i].ready, NULL) != 0)
			err_sys("pthread_cond_init error");
		if (pthread_mutex_init(&ti[i].mutex, NULL) != 0)
			err_sys("pthread_mutex_init error");
		if ((err = pthread_create(&tid[i], NULL, helper,
		  &ti[i])) != 0)
			err_exit(err, "pthread_create error");
	}

	for (;;) {
		if (poll(pfd, NQ, -1) < 0)
			err_sys("poll error");
		for (i = 0; i < NQ; i++) {
			if (pfd[i].revents & POLLIN) {
				if ((n = read(pfd[i].fd, &c, sizeof(char))) < 0)
					err_sys("read error");
				ti[i].m.mtext[ti[i].len] = 0;
				printf("queue id %d, message %s\n", qid[i],
				  ti[i].m.mtext);
				pthread_mutex_lock(&ti[i].mutex);
				pthread_cond_signal(&ti[i].ready);
				pthread_mutex_unlock(&ti[i].mutex);
			}
		}
	}

	exit(0);
}
