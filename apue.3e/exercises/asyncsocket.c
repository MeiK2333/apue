#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#if defined(BSD) || defined(MACOS) || defined(SOLARIS)
#include <sys/filio.h>
#endif

int
setasync(int sockfd)
{
	int n;

	if (fcntl(sockfd, F_SETOWN, getpid()) < 0)
		return(-1);
	n = 1;
	if (ioctl(sockfd, FIOASYNC, &n) < 0)
		return(-1);
	return(0);
}

int
clrasync(int sockfd)
{
	int n;

	n = 0;
	if (ioctl(sockfd, FIOASYNC, &n) < 0)
		return(-1);
	return(0);
}
