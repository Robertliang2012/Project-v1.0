#include <stdio.h>
#include <stdlib.h>

#include <libimobiledevice/libimobiledevice.h>

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
	
	if ((lockdownd_start_service
		 (client, "com.apple.mobile.notification_proxy",
		  &port) != LOCKDOWN_E_SUCCESS) || !port) {
		fprintf(stderr,
				"Could not start com.apple.mobile.notification_proxy!\n");
		goto leave_cleanup;
	}
	
	if (np_client_new(phone, port, &np) != NP_E_SUCCESS) {
		puts("Could not connect to notification_proxy!\n");
		goto leave_cleanup;
	}
	
	
	
	
	return 0;
}


