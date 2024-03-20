OUTPUT=bropen-scan

INPUTS=bropen_scan_main.c ../libsane-brother/bropen_yuv.c ../libsane-brother/bropen_types.c

BIN_INSTALL_DIR=/usr/local/bin
MAN_INSTALL_FILE=/usr/share/man/man1/bropen-scan.1

CFLAGS+=-Wall
CFLAGS+=-Wextra
CFLAGS+=-Werror
CFLAGS+=-O0 -g
#CFLAGS+=-std=c99

#CFLAGS+=-DBROPEN_G_DEBUG_USB
#CFLAGS+=-DBROPEN_G_DEBUG_COM
#CFLAGS+=-DBROPEN_G_DEBUG_DEC
#CFLAGS+=-DBROPEN_G_DEBUG_SCAN

build:
	${CC} ${CFLAGS} -I../libsane-brother ${INPUTS}  -l usb-1.0 -o ${OUTPUT}

clean:
	rm -f ${OUTPUT}

deinstall:
	rm -f ${BIN_INSTALL_DIR}/${OUTPUT}
	rm -f ${MAN_INSTALL_FILE}

distclean: clean deinstall

install: build
	cp ${OUTPUT} ${BIN_INSTALL_DIR}/${OUTPUT}
	cp bropen-scan.man ${MAN_INSTALL_FILE}
