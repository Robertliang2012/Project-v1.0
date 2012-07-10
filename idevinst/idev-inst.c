/*************************************************************************
	This program should merge idevicenistaller.c
	  and ifuse.c to accomplish the automation backup process.

						--Murray
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libimobiledevice/libimobiledevice.h>

int main()
{
	idevice_t	phone;
	char		*uuid = "a0a1a3ee9708777fcd7f052b8c849f1d5d9b0fb6";

	if (IDEVICE_E_SUCCESS != idevice_new(&phone, uuid)) {
		puts("I found no iPhone.");
		exit(1);
	}

	return 0;
}


