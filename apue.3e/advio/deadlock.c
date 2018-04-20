#include "apue.h"
#include <fcntl.h>

static void
lockabyte(const char *name, int fd, off_t offset)
{
	if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
		err_sys("%s: writew_lock error", name);
	printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int
main(void)
{
	int		fd;
	pid_t	pid;

	/*
	 * Create a file and write two bytes to it.
	 */
	if ((fd = creat("templock", FILE_MODE)) < 0)
		err_sys("creat error");
	if (write(fd, "ab", 2) != 2)
		err_sys("write error");

	TELL_WAIT();
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {			/* child */
		lockabyte("child", fd, 0);
		TELL_PARENT(getppid());
		WAIT_PARENT();
		lockabyte("child", fd, 1);
	} else {						/* parent */
		lockabyte("parent", fd, 1);
		TELL_CHILD(pid);
		WAIT_CHILD();
		lockabyte("parent", fd, 0);
	}
	exit(0);
}
