ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "freebsd"
  GETPW = getpw44bsd
  FMEM = fmemopen.o
  EXTRALIBS=-pthread
endif
ifeq "$(PLATFORM)" "linux"
  GETPW = getpwsvr4
  FMEM =
  EXTRALIBS=-pthread
endif
ifeq "$(PLATFORM)" "macos"
  GETPW = getpw44bsd
  FMEM = fmemopen.o
endif
ifeq "$(PLATFORM)" "solaris"
  GETPW = getpwsvr4
  FMEM =
endif

PROGS =	bo fifo1 getlogin goodexit longpath pendlock prtime sizepipe vfork3 zombie

all:	$(PROGS) asyncsocket.o $(FMEM) openmax.o sleep.o sleepus_poll.o sleepus_select.o $(GETPW)

%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o $(GETPW)

include $(ROOT)/Make.libapue.inc
