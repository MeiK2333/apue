#include "apue.h"
#include <errno.h>

#define	CL_OPEN "open"			/* client's request for server */
#define CS_OPEN "/tmp/opend.socket"	/* server's well-known name */

int		csopen(char *, int);
