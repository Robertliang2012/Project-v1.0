/*************************************************************************
    This program should merge idevicenistaller.c and ifuse.c
	    to accomplish the automation backup process.

                        --Murray
*************************************************************************/

int startup_config(void);

int main()
{
    startup_config();

	/* 
		Consequently, we need
			idevice_new()							-> to get phone
			lockdownd_client_new_with_handshake()	-> to get client
			lockdownd_start_service()				-> to get port

				client & port are both vital to communicate with iPhone

			instproxy_client_new()					-> to get ipc
		
				ipc is the most important to command iPhone.

		
			np_client_new()				-> to get np (which stands for notification)
			np_set_notify_callback()	-> to setup notification handler
			np_observe_notifications	-> to capture notifications
	 */
	

    return 0;
}


int startup_config()
{
    return 0;
}


