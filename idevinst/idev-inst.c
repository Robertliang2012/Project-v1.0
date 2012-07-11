#include <stdio.h>
#include <stdlib.h>

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>

int main()
{
	idevice_t	phone;
	char		*uuid = "a0a1a3ee9708777fcd7f052b8c849f1d5d9b0fb6";
	char		*client;
	const char  *service;
	uint16_t 	*port;

	if (IDEVICE_E_SUCCESS != idevice_new(&phone, uuid)) {
		puts("I found no iPhone.");
		exit(1);
	}

	if (LOCKDOWN_E_SUCCESS != lockdownd_client_new_with_handshake(phone, &client, "ideviceinstaller")) {
		puts("Couldn't connect to the lockdownd. Exiting")
		exit(1);
	}
	
	return 0;
}


