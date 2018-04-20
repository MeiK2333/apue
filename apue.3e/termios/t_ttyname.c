#include "apue.h"

int
main(void)
{
	char *name;
	if (isatty(0)) {
		name = ttyname(0);
		if (name == NULL)
			name = "undefined";
	} else {
		name = "not a tty";
	}
	printf("fd 0: %s\n", name);

	if (isatty(1)) {
		name = ttyname(1);
		if (name == NULL)
			name = "undefined";
	} else {
		name = "not a tty";
	}
	printf("fd 1: %s\n", name);

	if (isatty(2)) {
		name = ttyname(2);
		if (name == NULL)
			name = "undefined";
	} else {
		name = "not a tty";
	}
	printf("fd 2: %s\n", name);

	exit(0);
}
