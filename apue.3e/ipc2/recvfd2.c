#include "apue.h"
#include <sys/socket.h>		/* struct msghdr */
#include <sys/un.h>

#if defined(SCM_CREDS)			/* BSD interface */
#define CREDSTRUCT		cmsgcred
#define CR_UID			cmcred_uid
#define SCM_CREDTYPE	SCM_CREDS
#elif defined(SCM_CREDENTIALS)	/* Linux interface */
#define CREDSTRUCT		ucred
#define CR_UID			uid
#define CREDOPT			SO_PASSCRED
#define SCM_CREDTYPE	SCM_CREDENTIALS
#else
#error passing credentials is unsupported!
#endif

/* size of control buffer to send/recv one file descriptor */
#define RIGHTSLEN	CMSG_LEN(sizeof(int))
#define CREDSLEN	CMSG_LEN(sizeof(struct CREDSTRUCT))
#define	CONTROLLEN	(RIGHTSLEN + CREDSLEN)

static struct cmsghdr	*cmptr = NULL;		/* malloc'ed first time */

/*
 * Receive a file descriptor from a server process.  Also, any data
 * received is passed to (*userfunc)(STDERR_FILENO, buf, nbytes).
 * We have a 2-byte protocol for receiving the fd from send_fd().
 */
int
recv_ufd(int fd, uid_t *uidptr,
         ssize_t (*userfunc)(int, const void *, size_t))
{
	struct cmsghdr		*cmp;
	struct CREDSTRUCT	*credp;
	char				*ptr;
	char				buf[MAXLINE];
	struct iovec		iov[1];
	struct msghdr		msg;
	int					nr;
	int					newfd = -1;
	int					status = -1;
#if defined(CREDOPT)
	const int			on = 1;

	if (setsockopt(fd, SOL_SOCKET, CREDOPT, &on, sizeof(int)) < 0) {
		err_ret("setsockopt error");
		return(-1);
	}
#endif
	for ( ; ; ) {
		iov[0].iov_base = buf;
		iov[0].iov_len  = sizeof(buf);
		msg.msg_iov     = iov;
		msg.msg_iovlen  = 1;
		msg.msg_name    = NULL;
		msg.msg_namelen = 0;
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return(-1);
		msg.msg_control    = cmptr;
		msg.msg_controllen = CONTROLLEN;
		if ((nr = recvmsg(fd, &msg, 0)) < 0) {
			err_ret("recvmsg error");
			return(-1);
		} else if (nr == 0) {
			err_ret("connection closed by server");
			return(-1);
		}

		/*
		 * See if this is the final data with null & status.  Null
		 * is next to last byte of buffer; status byte is last byte.
		 * Zero status means there is a file descriptor to receive.
		 */
		for (ptr = buf; ptr < &buf[nr]; ) {
			if (*ptr++ == 0) {
				if (ptr != &buf[nr-1])
					err_dump("message format error");
 				status = *ptr & 0xFF;	/* prevent sign extension */
 				if (status == 0) {
					if (msg.msg_controllen != CONTROLLEN)
						err_dump("status = 0 but no fd");

					/* process the control data */
					for (cmp = CMSG_FIRSTHDR(&msg);
					  cmp != NULL; cmp = CMSG_NXTHDR(&msg, cmp)) {
						if (cmp->cmsg_level != SOL_SOCKET)
							continue;
						switch (cmp->cmsg_type) {
						case SCM_RIGHTS:
							newfd = *(int *)CMSG_DATA(cmp);
							break;
						case SCM_CREDTYPE:
							credp = (struct CREDSTRUCT *)CMSG_DATA(cmp);
							*uidptr = credp->CR_UID;
						}
					}
				} else {
					newfd = -status;
				}
				nr -= 2;
			}
		}
		if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)
			return(-1);
		if (status >= 0)	/* final data has arrived */
			return(newfd);	/* descriptor, or -status */
	}
}
