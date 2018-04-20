#include	"opend.h"

char	 errmsg[MAXLINE];
int		 oflag;
char	*pathname;

int
main(void)
{
	int		nread;
	char	buf[MAXLINE];

	for ( ; ; ) {	/* read arg buffer from client, process request */
		if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
			err_sys("read error on stream pipe");
		else if (nread == 0)
			break;		/* client has closed the stream pipe */
		handle_request(buf, nread, STDOUT_FILENO);
	}
	exit(0);
}
