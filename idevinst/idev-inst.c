/*************************************************************************
    This program should merge idevicenistaller.c
      and ifuse.c to accomplish the automation backup process.

                        --Murray
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/notification_proxy.h>


int		notified = 0;

static void notifier(const char *, void *);


int main()
{
    idevice_t			phone = NULL;
    char				*uuid = "a0a1a3ee9708777fcd7f052b8c849f1d5d9b0fb6";
    lockdownd_client_t	client = NULL;
<<<<<<< HEAD
	np_client_t 		np = NULL;
    const char			*noties[3] = {NP_APP_INSTALLED, NP_APP_UNINSTALLED, NULL};
	uint16_t 			port;
	int 				notification_expected = 0;
	
=======
		np_client_t np = NULL;
	uint16_t port = 0;

//    const char			*noties[3] = {NP_APP_INSTALLED, NP_APP_UNINSTALLED, NULL};

>>>>>>> origin/master
    idevice_new(&phone, uuid);
    lockdownd_client_new_with_handshake(phone, &client, "idev-inst");
    lockdownd_start_service(client, "com.apple.mobile.notification_proxy", &port);
    np_client_new(phone, port, &np);
	
    np_set_notify_callback(np, notifier, NULL);
	np_observe_notifications(np, noties);
	
	setbuf(stdout, NULL);
	
	notification_expected = 0;


	
    return 0;
}

	
	
	


static void notifier(const char *notification, void *unused)
{
	notified = 1;
}
