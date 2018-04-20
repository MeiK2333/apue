#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int
main(void)
{
	uint32_t		i = 0x04030201;
	unsigned char	*cp = (unsigned char *)&i;

	if (*cp == 1)
		printf("little-endian\n");
	else if (*cp == 4)
		printf("big-endian\n");
	else
		printf("who knows?\n");
	exit(0);
}
