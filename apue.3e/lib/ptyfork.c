#include "apue.h"
#include <termios.h>

pid_t
pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
		 const struct termios *slave_termios,
		 const struct winsize *slave_winsize)
{
	int		fdm, fds;
	pid_t	pid;
	char	pts_name[20];

	if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0)
		err_sys("can't open master pty: %s, error %d", pts_name, fdm);

	if (slave_name != NULL) {
		/*
		 * Return name of slave.  Null terminate to handle case
		 * where strlen(pts_name) > slave_namesz.
		 */
		strncpy(slave_name, pts_name, slave_namesz);
		slave_name[slave_namesz - 1] = '\0';
	}

	if ((pid = fork()) < 0) {
		return(-1);
	} else if (pid == 0) {		/* child */
		if (setsid() < 0)
			err_sys("setsid error");

		/*
		 * System V acquires controlling terminal on open().
		 */
		if ((fds = ptys_open(pts_name)) < 0)
			err_sys("can't open slave pty");
		close(fdm);		/* all done with master in child */

#if	defined(BSD)
		/*
		 * TIOCSCTTY is the BSD way to acquire a controlling terminal.
		 */
		if (ioctl(fds, TIOCSCTTY, (char *)0) < 0)
			err_sys("TIOCSCTTY error");
#endif
		/*
		 * Set slave's termios and window size.
		 */
		if (slave_termios != NULL) {
			if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
				err_sys("tcsetattr error on slave pty");
		}
		if (slave_winsize != NULL) {
			if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
				err_sys("TIOCSWINSZ error on slave pty");
		}

		/*
		 * Slave becomes stdin/stdout/stderr of child.
		 */
		if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
			err_sys("dup2 error to stdin");
		if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
			err_sys("dup2 error to stdout");
		if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
			err_sys("dup2 error to stderr");
		if (fds != STDIN_FILENO && fds != STDOUT_FILENO &&
		  fds != STDERR_FILENO)
			close(fds);
		return(0);		/* child returns 0 just like fork() */
	} else {					/* parent */
		*ptrfdm = fdm;	/* return fd of master */
		return(pid);	/* parent returns pid of child */
	}
}
