ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS =	add2 add2stdio devzero myuclc pipe1 pipe2 pipe4 popen1 popen2 tshm

all:	$(PROGS) popen.o slock.o tellwait.o

%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

slock.o:	slock.c slock.h

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libapue.inc
