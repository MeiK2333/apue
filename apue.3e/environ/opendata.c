#include	<stdio.h>

FILE *
open_data(void)
{
	FILE	*fp;
	char	databuf[BUFSIZ];  /* setvbuf makes this the stdio buffer */

	if ((fp = fopen("datafile", "r")) == NULL)
		return(NULL);
	if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0)
		return(NULL);
	return(fp);		/* error */
}
