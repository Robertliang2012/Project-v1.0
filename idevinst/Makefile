all: idev-inst ideviceinstaller ifuse

idev-inst:	idev-inst.c
	gcc -Wall -O2 idev-inst.c -o idev-inst -limobiledevice

ideviceinstaller:	ideviceinstaller.c
	gcc -Wall -O2 ideviceinstaller.c -o ideviceinstaller -limobiledevice -DHAVE_CONFIG_H -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/libxml2 -I/usr/lib/libzip/include -I. -L/usr/local/lib -limobiledevice -lplist -lusbmuxd -lssl -lcrypto -lzip -lz

ifuse:	ifuse.c
	gcc -DHAVE_CONFIG_H -I. -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -I/usr/local/include -I/usr/include/libxml2   -I/usr/include/libxml2 -O2 -o ifuse ifuse.c -L/usr/local/lib -limobiledevice -lplist -lusbmuxd -lssl -lcrypto -lfuse

clean:
	rm ideviceinstaller idev-inst ifuse
	
