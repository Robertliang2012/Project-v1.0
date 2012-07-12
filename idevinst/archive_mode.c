else if (archive_mode) {

        /* the place where we should merge into the list-app loop    --Murray */
		
		
        char	*copy_path = NULL;
        int		remove_after_copy = 0;
        int		skip_uninstall = 1;
        int		app_only = 0;
        plist_t client_opts = NULL;

        /* look for options */
        if (options) {
            char	*opts = strdup(options);
            char	*elem = strtok(opts, ",");
            while (elem) {
                if (!strcmp(elem, "uninstall")) {
                    skip_uninstall = 0;
                } else if (!strcmp(elem, "app_only")) {
                    app_only = 1;
                	else()
				if ((strlen(elem) > 5) && !strncmp(elem, "copy=", 5)) {
                    copy_path = strdup(elem + 5);
                } else if (!strcmp(elem, "remove")) {
                    remove_after_copy = 1;
                }
                elem = strtok(NULL, ",");
            }
        }

        skip_uninstall = 1; /* Feed in some correct parameters to force it behave as our wish.	--Murray */
        app_only = 0;
        if (skip_uninstall || app_only) {
            client_opts = instproxy_client_options_new();
            if (skip_uninstall) {
                instproxy_client_options_add(client_opts, "SkipUninstall", 1, NULL);
            }
            if (app_only) {
                instproxy_client_options_add(client_opts, "ArchiveType", "ApplicationOnly", NULL);
            }
        }

        if (copy_path) {
            struct stat fst;
            if (stat(copy_path, &fst) != 0) {
                fprintf(stderr, "ERROR: stat: %s: %s\n", copy_path, strerror(errno));
                free(copy_path);
                goto leave_cleanup;
            }

            if (!S_ISDIR(fst.st_mode)) {
                fprintf(stderr, "ERROR: '%s' is not a directory as expected.\n", copy_path);
                free(copy_path);
                goto leave_cleanup;
            }

            port = 0;
            if ((lockdownd_start_service(client, "com.apple.afc", &port) != LOCKDOWN_E_SUCCESS) || !port) {
                fprintf(stderr, "Could not start com.apple.afc!\n");
                free(copy_path);
                goto leave_cleanup;
            }

            lockdownd_client_free(client);
            client = NULL;

            /*
                    afc_client_new() is a libimobiledevice API.
                    Check it in libimobiledevice/src/afc.c.
                                                            --Murray
             */
            if (afc_client_new(phone, port, &afc) != INSTPROXY_E_SUCCESS) {
                fprintf(stderr, "Could not connect to AFC!\n");
                goto leave_cleanup;
            }
        }

#ifdef HAVE_LIBIMOBILEDEVICE_1_1
        instproxy_archive(ipc, appid, client_opts, status_cb, NULL);
#else
        instproxy_archive(ipc, appid, client_opts, status_cb);
#endif
        instproxy_client_options_free(client_opts);
        wait_for_op_complete = 1;
        if (skip_uninstall) {
            notification_expected = 0;
        } else {
            notification_expected = 1;
        }

        do_wait_when_needed();

        if (copy_path) {
            if (err_occured) {
                afc_client_free(afc);
                afc = NULL;
                goto leave_cleanup;
            }
            FILE		*f = NULL;
            uint64_t	af = 0;
            /* local filename */
            char *localfile = NULL;
            if (asprintf(&localfile, "%s/%s.ipa", copy_path, appid) < 0) {
                fprintf(stderr, "Out of memory!?\n");
                goto leave_cleanup;
            }
            free(copy_path);

            f = fopen(localfile, "w");
            if (!f) {
                fprintf(stderr, "ERROR: fopen: %s: %s\n", localfile, strerror(errno));
                free(localfile);
                goto leave_cleanup;
            }

            /* remote filename */
            char *remotefile = NULL;
            if (asprintf(&remotefile, "%s/%s.zip", APPARCH_PATH, appid) < 0) {
                fprintf(stderr, "Out of memory!?\n");
                goto leave_cleanup;
            }

            uint32_t	fsize = 0;
            char		**fileinfo = NULL;
            if ((afc_get_file_info(afc, remotefile, &fileinfo) != AFC_E_SUCCESS) || !fileinfo) {
                fprintf(stderr, "ERROR getting AFC file info for '%s' on device!\n", remotefile);
                fclose(f);
                free(remotefile);
                free(localfile);
                goto leave_cleanup;
            }

            int i;
            for (i = 0; fileinfo[i]; i += 2) {
                if (!strcmp(fileinfo[i], "st_size")) {
                    fsize = atoi(fileinfo[i + 1]);
                    break;
                }
            }
            i = 0;
            while (fileinfo[i]) {
                free(fileinfo[i]);
                i++;
            }
            free(fileinfo);

            if (fsize == 0) {
                fprintf(stderr, "Hm... remote file length could not be determined. Cannot copy.\n");
                fclose(f);
                free(remotefile);
                free(localfile);
                goto leave_cleanup;
            }

            if ((afc_file_open(afc, remotefile, AFC_FOPEN_RDONLY, &af) != AFC_E_SUCCESS) || !af) {
                fclose(f);
                fprintf(stderr, "ERROR: could not open '%s' on device for reading!\n", remotefile);
                free(remotefile);
                free(localfile);
                goto leave_cleanup;
            }

            /* copy file over */
            printf("Copying '%s' --> '%s'\n", remotefile, localfile);
            free(remotefile);
            free(localfile);

            uint32_t	amount = 0;
            uint32_t	total = 0;
            char		buf[8192];

            do {
                /*
                        afc_clien_read() is a libimobiledevice API.
                        Check it in libimobiledevice/src/afc.c.
                                                            --Murray
                 */
                if (afc_file_read(afc, af, buf, sizeof (buf), &amount) != AFC_E_SUCCESS) {
                    fprintf(stderr, "AFC Read error!\n");
                    break;
                }

                if (amount > 0) {
                    size_t written = fwrite(buf, 1, amount, f);
                    if (written != amount) {
                        fprintf(stderr, "Error when writing %d bytes to local file!\n", amount);
                        break;
                    }
                    total += written;
                }
            } while (amount > 0);

            afc_file_close(afc, af);
            fclose(f);

            printf("done.\n");
            if (total != fsize) {
                fprintf(stderr, "WARNING: remote and local file sizes don't match (%d != %d)\n", fsize, total);
                if (remove_after_copy) {
                    fprintf(stderr, "NOTE: archive file will NOT be removed from device\n");
                    remove_after_copy = 0;
                }
            }

            if (remove_after_copy) {
                /* remove archive if requested */
                printf("Removing '%s'\n", appid);
                archive_mode = 0;
                remove_archive_mode = 1;
                free(options);
                options = NULL;
                if (LOCKDOWN_E_SUCCESS != lockdownd_client_new_with_handshake(phone, &client, "ideviceinstaller")) {
                    fprintf(stderr, "Could not connect to lockdownd. Exiting.\n");
                    goto leave_cleanup;
                }
                goto run_again;
            }
        }
        goto leave_cleanup;
    }
