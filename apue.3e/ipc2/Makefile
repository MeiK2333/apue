ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "freebsd"
  EXTRALIBS=-pthread
endif
ifeq "$(PLATFORM)" "linux"
  EXTRALIBS=-pthread
endif
ifeq "$(PLATFORM)" "solaris"
  EXTRALIBS=-lsocket -lnsl
endif

PROGS =	bindunix pollmsg sendmsg
FDPASS =

ifeq "$(PLATFORM)" "linux"
	FDPASS = recvfd2.o sendfd2.o
endif
ifeq "$(PLATFORM)" "freebsd"
	FDPASS = recvfd2.o sendfd2.o
endif

all:	$(PROGS) $(FDPASS)
	for i in open opend open.fe opend.fe; do \
		(cd $$i && $(MAKE) ) || exit 1; \
	done

%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o
	for i in open opend open.fe opend.fe; do \
		(cd $$i && $(MAKE) clean) || exit 1; \
	done

include $(ROOT)/Make.libapue.inc
