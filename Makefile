CFLAGS=-lmenu -lncurses

radiotower: radiotower.c
	${CC} -o radiotower radiotower.c $(CFLAGS)

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f radiotower ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/radiotower
	mkdir -p /etc/radiotower
	cp -f sign_acii.txt /etc/radiotower
	cp -f tower_acii.txt /etc/radiotower

script: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f rtopen.sh ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/rtopen.sh

clean: 
	rm radiotower

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/radiotower
	rm -f ${DESTDIR}${PREFIX}/bin/rtopen.sh

.PHONY: install clean script uninstall
