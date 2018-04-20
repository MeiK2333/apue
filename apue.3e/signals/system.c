#include	<sys/wait.h>
#include	<errno.h>
#include	<signal.h>
#include	<unistd.h>

int
system(const char *cmdstring)	/* with appropriate signal handling */
{
	pid_t				pid;
	int					status;
	struct sigaction	ignore, saveintr, savequit;
	sigset_t			chldmask, savemask;

	if (cmdstring == NULL)
		return(1);		/* always a command processor with UNIX */

	ignore.sa_handler = SIG_IGN;	/* ignore SIGINT and SIGQUIT */
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	if (sigaction(SIGINT, &ignore, &saveintr) < 0)
		return(-1);
	if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
		return(-1);
	sigemptyset(&chldmask);			/* now block SIGCHLD */
	sigaddset(&chldmask, SIGCHLD);
	if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
		return(-1);

	if ((pid = fork()) < 0) {
		status = -1;	/* probably out of processes */
	} else if (pid == 0) {			/* child */
		/* restore previous signal actions & reset signal mask */
		sigaction(SIGINT, &saveintr, NULL);
		sigaction(SIGQUIT, &savequit, NULL);
		sigprocmask(SIG_SETMASK, &savemask, NULL);

		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(127);		/* exec error */
	} else {						/* parent */
		while (waitpid(pid, &status, 0) < 0)
			if (errno != EINTR) {
				status = -1; /* error other than EINTR from waitpid() */
				break;
			}
	}

	/* restore previous signal actions & reset signal mask */
	if (sigaction(SIGINT, &saveintr, NULL) < 0)
		return(-1);
	if (sigaction(SIGQUIT, &savequit, NULL) < 0)
		return(-1);
	if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
		return(-1);

	return(status);
}
