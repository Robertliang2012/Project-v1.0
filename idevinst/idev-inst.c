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

#define DIR_HOME    "/beckie"
#define DIR_APPDATA "/beckie/appdata"

int mkworkdir(void);

int main()
{
    mkworkdir();


    return 0;
}


int mkworkdir()
{
    struct stat st;
    int			n;

    if (chdir(DIR_HOME) < 0) {
        perror("chdir");
        exit(1);
    }

    n = stat(DIR_APPDATA, &st);
    if (n < 0) {
        if (errno != ENOENT) {
            perror("stat");
            exit(1);
        } else {
            mkdir(DIR_APPDATA, 0755);
        }
    } else if (st.st_mode != 0755) {
        chmod(DIR_APPDATA, 0755);
    }

    return n;
}


