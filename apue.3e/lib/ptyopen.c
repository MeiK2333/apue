#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#if defined(SOLARIS)
#include <stropts.h>
#endif

int
ptym_open(char *pts_name, int pts_namesz)
{
	char	*ptr;
	int		fdm, err;

	if ((fdm = posix_openpt(O_RDWR)) < 0)
		return(-1);
	if (grantpt(fdm) < 0)		/* grant access to slave */
		goto errout;
	if (unlockpt(fdm) < 0)		/* clear slave's lock flag */
		goto errout;
	if ((ptr = ptsname(fdm)) == NULL)	/* get slave's name */
		goto errout;

	/*
	 * Return name of slave.  Null terminate to handle
	 * case where strlen(ptr) > pts_namesz.
	 */
	strncpy(pts_name, ptr, pts_namesz);
	pts_name[pts_namesz - 1] = '\0';
	return(fdm);			/* return fd of master */
errout:
	err = errno;
	close(fdm);
	errno = err;
	return(-1);
}

int
ptys_open(char *pts_name)
{
	int fds;
#if defined(SOLARIS)
	int err, setup;
#endif

	if ((fds = open(pts_name, O_RDWR)) < 0)
		return(-1);

#if defined(SOLARIS)
	/*
	 * Check if stream is already set up by autopush facility.
	 */
	if ((setup = ioctl(fds, I_FIND, "ldterm")) < 0)
		goto errout;

	if (setup == 0) {
		if (ioctl(fds, I_PUSH, "ptem") < 0)
			goto errout;
		if (ioctl(fds, I_PUSH, "ldterm") < 0)
			goto errout;
		if (ioctl(fds, I_PUSH, "ttcompat") < 0) {
errout:
			err = errno;
			close(fds);
			errno = err;
			return(-1);
		}
	}
#endif
	return(fds);
}
