#include	"opend.h"
#include	<poll.h>

#define NALLOC	10	/* # pollfd structs to alloc/realloc */

static struct pollfd *
grow_pollfd(struct pollfd *pfd, int *maxfd)
{
	int				i;
	int				oldmax = *maxfd;
	int				newmax = oldmax + NALLOC;

	if ((pfd = realloc(pfd, newmax * sizeof(struct pollfd))) == NULL)
		err_sys("realloc error");
	for (i = oldmax; i < newmax; i++) {
		pfd[i].fd = -1;
		pfd[i].events = POLLIN;
		pfd[i].revents = 0;
	}
	*maxfd = newmax;
	return(pfd);
}

void
loop(void)
{
	int				i, listenfd, clifd, nread;
	char			buf[MAXLINE];
	uid_t			uid;
	struct pollfd	*pollfd;
	int				numfd = 1;
	int				maxfd = NALLOC;

	if ((pollfd = malloc(NALLOC * sizeof(struct pollfd))) == NULL)
		err_sys("malloc error");
	for (i = 0; i < NALLOC; i++) {
		pollfd[i].fd = -1;
		pollfd[i].events = POLLIN;
		pollfd[i].revents = 0;
	}

	/* obtain fd to listen for client requests on */
	if ((listenfd = serv_listen(CS_OPEN)) < 0)
		log_sys("serv_listen error");
	client_add(listenfd, 0);	/* we use [0] for listenfd */
	pollfd[0].fd = listenfd;

	for ( ; ; ) {
		if (poll(pollfd, numfd, -1) < 0)
			log_sys("poll error");

		if (pollfd[0].revents & POLLIN) {
			/* accept new client request */
			if ((clifd = serv_accept(listenfd, &uid)) < 0)
				log_sys("serv_accept error: %d", clifd);
			client_add(clifd, uid);

			/* possibly increase the size of the pollfd array */
			if (numfd == maxfd)
				pollfd = grow_pollfd(pollfd, &maxfd);
			pollfd[numfd].fd = clifd;
			pollfd[numfd].events = POLLIN;
			pollfd[numfd].revents = 0;
			numfd++;
			log_msg("new connection: uid %d, fd %d", uid, clifd);
		}

		for (i = 1; i < numfd; i++) {
			if (pollfd[i].revents & POLLHUP) {
				goto hungup;
			} else if (pollfd[i].revents & POLLIN) {
				/* read argument buffer from client */
				if ((nread = read(pollfd[i].fd, buf, MAXLINE)) < 0) {
					log_sys("read error on fd %d", pollfd[i].fd);
				} else if (nread == 0) {
hungup:
					/* the client closed the connection */
					log_msg("closed: uid %d, fd %d",
					  client[i].uid, pollfd[i].fd);
					client_del(pollfd[i].fd);
					close(pollfd[i].fd);
					if (i < (numfd-1)) {
						/* pack the array */
						pollfd[i].fd = pollfd[numfd-1].fd;
						pollfd[i].events = pollfd[numfd-1].events;
						pollfd[i].revents = pollfd[numfd-1].revents;
						i--;	/* recheck this entry */
					}
					numfd--;
				} else {		/* process client's request */
					handle_request(buf, nread, pollfd[i].fd,
					  client[i].uid);
				}
			}
		}
	}
}
