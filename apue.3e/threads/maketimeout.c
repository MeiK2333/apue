#include <sys/time.h>
#include <stdlib.h>

void
maketimeout(struct timespec *tsp, long minutes)
{
	struct timeval now;

	/* get the current time */
	gettimeofday(&now, NULL);
	tsp->tv_sec = now.tv_sec;
	tsp->tv_nsec = now.tv_usec * 1000; /* usec to nsec */
	/* add the offset to get timeout value */
	tsp->tv_sec += minutes * 60;
}
