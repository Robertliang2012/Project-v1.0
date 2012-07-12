/*************************************************************************
    This program should merge idevicenistaller.c
      and ifuse.c to accomplish the automation backup process.

                        --Murray
*************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define DIR_HOME	"/beckie"
#define DIR_APPDATA	"/beckie/appdata"

int cdworkdir(void);

int main()
{
	cdworkdir();
    return 0;
}

int cdworkdir()
{
	struct stat	unused;
	int	n;

	if (chdir(DIR_HOME) < 0)	{
		perror("chdir");
		exit(1);
	}

	n = stat(DIR_APPDATA, &unused);
	if (n < 0 && errno == ENOENT)	{
		mkdir(DIR_APPDATA, 0644);
	}
	
	return n;
}
