/* Create a named stream pipe.  Called by server on initialization. */

#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>

int			/* returns 0 if all OK, -1 if error (with errno set) */
ns_pipe(const char *name, int fd[2])
{
	int					len;
	struct sockaddr_un	unix_addr;

	if (fd_pipe(fd) < 0)		/* create unnamed stream pipe */
		return(-1);

	unlink(name);	/* remove the name, if it already exists */

	memset(&unix_addr, 0, sizeof(unix_addr));
	unix_addr.sun_family = AF_UNIX;
	strcpy(unix_addr.sun_path, name);
	len = strlen(unix_addr.sun_path) + sizeof(unix_addr.sun_family);

	return(bind(fd[0], (struct sockaddr *) &unix_addr, len));
						/* fd[0] has the name bound to it */
}
