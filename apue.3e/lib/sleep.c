#include "apue.h"

static void
sig_alrm(int signo)
{
	/* nothing to do, just returning wakes up sigsuspend() */
}

unsigned int
sleep(unsigned int seconds)
{
	struct sigaction	newact, oldact;
	sigset_t			newmask, oldmask, suspmask;
	unsigned int		unslept;

	/* set our handler, save previous information */
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);

	/* block SIGALRM and save current signal mask */
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	alarm(seconds);
	suspmask = oldmask;

	/* make sure SIGALRM isn't blocked */
	sigdelset(&suspmask, SIGALRM);

	/* wait for any signal to be caught */
	sigsuspend(&suspmask);

	/* some signal has been caught, SIGALRM is now blocked */

	unslept = alarm(0);

	/* reset previous action */
	sigaction(SIGALRM, &oldact, NULL);

	/* reset signal mask, which unblocks SIGALRM */
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	return(unslept);
}
