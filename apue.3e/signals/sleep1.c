#include	<signal.h>
#include	<unistd.h>

static void
sig_alrm(int signo)
{
	/* nothing to do, just return to wake up the pause */
}

unsigned int
sleep1(unsigned int seconds)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		return(seconds);
	alarm(seconds);		/* start the timer */
	pause();			/* next caught signal wakes us up */
	return(alarm(0));	/* turn off timer, return unslept time */
}
