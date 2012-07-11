/*************************************************************************
    This program should merge idevicenistaller.c
      and ifuse.c to accomplish the automation backup process.

                        --Murray
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>


int		notified = 0;


static void notifier(const char *, void *);


int main()
{
    idevice_t			phone = NULL;
    char				*uuid = "a0a1a3ee9708777fcd7f052b8c849f1d5d9b0fb6";
    lockdownd_client_t	client = NULL;
    const char			*noties[3] = {NP_APP_INSTALLED, NP_APP_UNINSTALLED, NULL};

    idevice_new(&phone, uuid);
    lockdownd_client_new_with_handshake(phone, &client, "idev-inst");
    lockdownd_start_service(client, "com.apple.mobile.notification_proxy", &port);
    np_client_new(phone, port, &np);

    np_set_notify_callback(np, notifier, NULL);

    return 0;
}


static void notifier(const char *notification, void *unused)
{
	notified = 1;
}
