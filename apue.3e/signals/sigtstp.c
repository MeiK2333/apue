#include "apue.h"

#define	BUFFSIZE	1024

static void
sig_tstp(int signo)	/* signal handler for SIGTSTP */
{
	sigset_t	mask;

	/* ... move cursor to lower left corner, reset tty mode ... */

	/*
	 * Unblock SIGTSTP, since it's blocked while we're handling it.
	 */
	sigemptyset(&mask);
	sigaddset(&mask, SIGTSTP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	signal(SIGTSTP, SIG_DFL);	/* reset disposition to default */

	kill(getpid(), SIGTSTP);	/* and send the signal to ourself */

	/* we won't return from the kill until we're continued */

	signal(SIGTSTP, sig_tstp);	/* reestablish signal handler */

	/* ... reset tty mode, redraw screen ... */
}

int
main(void)
{
	int		n;
	char	buf[BUFFSIZE];

	/*
	 * Only catch SIGTSTP if we're running with a job-control shell.
	 */
	if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
		signal(SIGTSTP, sig_tstp);

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

	exit(0);
}
