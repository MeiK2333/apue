#include "apue.h"
#include <fcntl.h>
#include <errno.h>

void
sigint(int signo)
{
}

int
main(void)
{
	pid_t pid1, pid2, pid3;
	int fd;

	setbuf(stdout, NULL);
	signal_intr(SIGINT, sigint);

	/*
	 * Create a file.
	 */
	if ((fd = open("lockfile", O_RDWR|O_CREAT, 0666)) < 0)
		err_sys("can't open/create lockfile");

	/*
	 * Read-lock the file.
	 */
	if ((pid1 = fork()) < 0) {
		err_sys("fork failed");
	} else if (pid1 == 0) {	/* child */
		if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)
			err_sys("child 1: can't read-lock file");
		printf("child 1: obtained read lock on file\n");
		pause();
		printf("child 1: exit after pause\n");
		exit(0);
	} else {		/* parent */
		sleep(2);
	}

	/*
	 * Parent continues ... read-lock the file again.
	 */
	if ((pid2 = fork()) < 0) {
		err_sys("fork failed");
	} else if (pid2 == 0) {	/* child */
		if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)
			err_sys("child 2: can't read-lock file");
		printf("child 2: obtained read lock on file\n");
		pause();
		printf("child 2: exit after pause\n");
		exit(0);
	} else {		/* parent */
		sleep(2);
	}

	/*
	 * Parent continues ... block while trying to write-lock
	 * the file.
	 */
	if ((pid3 = fork()) < 0) {
		err_sys("fork failed");
	} else if (pid3 == 0) {	/* child */
		if (lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0)
			printf("child 3: can't set write lock: %s\n",
			  strerror(errno));
		printf("child 3 about to block in write-lock...\n");
		if (lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 0) < 0)
			err_sys("child 3: can't write-lock file");
		printf("child 3 returned and got write lock????\n");
		pause();
		printf("child 3: exit after pause\n");
		exit(0);
	} else {		/* parent */
		sleep(2);
	}

	/*
	 * See if a pending write lock will block the next
	 * read-lock attempt.
	 */
	if (lock_reg(fd, F_SETLK, F_RDLCK, 0, SEEK_SET, 0) < 0)
		printf("parent: can't set read lock: %s\n",
		  strerror(errno));
	else
		printf("parent: obtained additional read lock while"
		  " write lock is pending\n");
	printf("killing child 1...\n");
	kill(pid1, SIGINT);
	printf("killing child 2...\n");
	kill(pid2, SIGINT);
	printf("killing child 3...\n");
	kill(pid3, SIGINT);
	exit(0);
}
