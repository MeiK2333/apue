#include "apue.h"
#include <termios.h>

int
main(void)
{
	struct termios	term;
	long			vdisable;

	if (isatty(STDIN_FILENO) == 0)
		err_quit("standard input is not a terminal device");

	if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
		err_quit("fpathconf error or _POSIX_VDISABLE not in effect");

	if (tcgetattr(STDIN_FILENO, &term) < 0)	/* fetch tty state */
		err_sys("tcgetattr error");

	term.c_cc[VINTR] = vdisable;	/* disable INTR character */
	term.c_cc[VEOF]  = 2;			/* EOF is Control-B */

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
		err_sys("tcsetattr error");

	exit(0);
}
