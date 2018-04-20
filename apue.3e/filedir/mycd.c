#include "apue.h"

int
main(void)
{
	if (chdir("/tmp") < 0)
		err_sys("chdir failed");
	printf("chdir to /tmp succeeded\n");
	exit(0);
}
