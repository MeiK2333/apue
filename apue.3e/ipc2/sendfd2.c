#include "apue.h"
#include <sys/socket.h>

#if defined(SCM_CREDS)			/* BSD interface */
#define CREDSTRUCT		cmsgcred
#define SCM_CREDTYPE	SCM_CREDS
#elif defined(SCM_CREDENTIALS)	/* Linux interface */
#define CREDSTRUCT		ucred
#define SCM_CREDTYPE	SCM_CREDENTIALS
#else
#error passing credentials is unsupported!
#endif

/* size of control buffer to send/recv one file descriptor */
#define RIGHTSLEN	CMSG_LEN(sizeof(int))
#define CREDSLEN	CMSG_LEN(sizeof(struct CREDSTRUCT))
#define	CONTROLLEN	(RIGHTSLEN + CREDSLEN)

static struct cmsghdr	*cmptr = NULL;	/* malloc'ed first time */

/*
 * Pass a file descriptor to another process.
 * If fd<0, then -fd is sent back instead as the error status.
 */
int
send_fd(int fd, int fd_to_send)
{
	struct CREDSTRUCT	*credp;
	struct cmsghdr		*cmp;
	struct iovec		iov[1];
	struct msghdr		msg;
	char				buf[2];	/* send_fd/recv_ufd 2-byte protocol */

	iov[0].iov_base = buf;
	iov[0].iov_len  = 2;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;
	msg.msg_name    = NULL;
	msg.msg_namelen = 0;
	msg.msg_flags = 0;
	if (fd_to_send < 0) {
		msg.msg_control    = NULL;
		msg.msg_controllen = 0;
		buf[1] = -fd_to_send;	/* nonzero status means error */
		if (buf[1] == 0)
			buf[1] = 1;	/* -256, etc. would screw up protocol */
	} else {
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return(-1);
		msg.msg_control    = cmptr;
		msg.msg_controllen = CONTROLLEN;
		cmp = cmptr;
		cmp->cmsg_level  = SOL_SOCKET;
		cmp->cmsg_type   = SCM_RIGHTS;
		cmp->cmsg_len    = RIGHTSLEN;
		*(int *)CMSG_DATA(cmp) = fd_to_send;	/* the fd to pass */
		cmp = CMSG_NXTHDR(&msg, cmp);
		cmp->cmsg_level  = SOL_SOCKET;
		cmp->cmsg_type   = SCM_CREDTYPE;
		cmp->cmsg_len    = CREDSLEN;
		credp = (struct CREDSTRUCT *)CMSG_DATA(cmp);
#if defined(SCM_CREDENTIALS)
		credp->uid = geteuid();
		credp->gid = getegid();
		credp->pid = getpid();
#endif
		buf[1] = 0;		/* zero status means OK */
	}
	buf[0] = 0;			/* null byte flag to recv_ufd() */
	if (sendmsg(fd, &msg, 0) != 2)
		return(-1);
	return(0);
}
