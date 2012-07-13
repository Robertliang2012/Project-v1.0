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

            instproxy_client_new()	-> to get ipc

                ipc is the most important to command iPhone.


            afc_client_new()	->	get afc
            afc_get_file_info()	->	get fileinfo; fileinfo contains file size
            afc_file_open()		->	get af; af is the file handler in libimobiledevice
            afc_file_read()		->	actually read file
            afc_file_close()	->	close it


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


