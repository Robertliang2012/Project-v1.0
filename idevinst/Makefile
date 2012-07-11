all: idev-inst ideviceinstaller

idev-inst:	idev-inst.c
	gcc -Wall -O2 idev-inst.c -o idev-inst -limobiledevice

ideviceinstaller:	ideviceinstaller.c
	gcc -Wall -O2 ideviceinstaller.c -o ideviceinstaller -limobiledevice -DHAVE_CONFIG_H -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/libxml2 -I/usr/lib/libzip/include -I. -lplist -L/usr/local/lib -limobiledevice -lplist -lusbmuxd -lssl -lcrypto    -lplist   -lzip -lz

clean:
	rm ideviceinstaller idev-inst
	