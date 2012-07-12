	list_apps_mode = 1; /* We force it stay in list_apps_mode.	--Murray */
    if (list_apps_mode) {
        int		xml_mode = 0;
        plist_t client_opts = instproxy_client_options_new();
        instproxy_client_options_add(client_opts, "ApplicationType", "User", NULL);
        instproxy_error_t	err;
        plist_t				apps = NULL;

        /* look for options */
        if (options) {
            char	*opts = strdup(options);
            char	*elem = strtok(opts, ",");
            while (elem) {
                if (!strcmp(elem, "list_system")) {
                    if (!client_opts) {
                        client_opts = instproxy_client_options_new();
                    }
                    instproxy_client_options_add(client_opts, "ApplicationType", "System", NULL);
                } else if (!strcmp(elem, "list_all")) {
                    instproxy_client_options_free(client_opts);
                    client_opts = NULL;
                } else if (!strcmp(elem, "list_user")) {
                    /* do nothing, we're already set */
                } else if (!strcmp(elem, "xml")) {
                    xml_mode = 1;
                }
                elem = strtok(NULL, ",");
            }
        }

        err = instproxy_browse(ipc, client_opts, &apps);
        instproxy_client_options_free(client_opts);
        if (err != INSTPROXY_E_SUCCESS) {
            fprintf(stderr, "ERROR: instproxy_browse returned %d\n", err);
            goto leave_cleanup;
        }
        if (!apps || (plist_get_node_type(apps) != PLIST_ARRAY)) {
            fprintf(stderr,
                    "ERROR: instproxy_browse returnd an invalid plist!\n");
            goto leave_cleanup;
        }

        xml_mode = 0;
        if (xml_mode) {
            char		*xml = NULL;
            uint32_t	len = 0;

            plist_to_xml(apps, &xml, &len);
            if (xml) {
                puts(xml);
                free(xml);
            }
            plist_free(apps);
            goto leave_cleanup;
        }
        printf("Total: %d apps\n", plist_array_get_size(apps));
        uint32_t i = 0;
        for (i = 0; i < plist_array_get_size(apps); i++) {

            /* this is the place where we should add our codes    --Murray */

            plist_t app = plist_array_get_item(apps, i);
            plist_t p_appid =
                plist_dict_get_item(app, "CFBundleIdentifier");
            char	*s_appid = NULL;
            char	*s_dispName = NULL;
            char	*s_version = NULL;
            char	syscmd[1024] = "";

            plist_t dispName =
                plist_dict_get_item(app, "CFBundleDisplayName");
            plist_t version = plist_dict_get_item(app, "CFBundleVersion");

            if (p_appid) {
                plist_get_string_val(p_appid, &s_appid);
				
            }
            if (!s_appid) {
                fprintf(stderr, "ERROR: Failed to get APPID!\n");
                break;
            }

            if (dispName) {
                plist_get_string_val(dispName, &s_dispName);
            }
            if (version) {
                plist_get_string_val(version, &s_version);
            }

            if (!s_dispName) {
                s_dispName = strdup(s_appid);
            }
            if (s_version) {
                printf("%s - %s %s\n", s_appid, s_dispName, s_version);
                free(s_version);
            } else {
                printf("%s - %s\n", s_appid, s_dispName);
            }

            sprintf(syscmd, "ifuse appdata --appid %s", s_appid);
            printf("DEBUG! cmd: %s\n", syscmd);

            system(syscmd);
            sprintf(syscmd, "cp -dprvf appdata app-%s", s_appid);
            printf("DEBUG! cmd: %s\n", syscmd);
            system(syscmd);
            system("umount appdata");

            free(s_dispName);
            free(s_appid);
        }
        plist_free(apps);
    }
