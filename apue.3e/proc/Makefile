ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ACCT=
ACCTFLAGS=
ifeq "$(PLATFORM)" "linux"
  ACCT=pracct
  ACCTFLAGS = -DHAS_AXSIG -DHAS_ACORE
endif
ifeq "$(PLATFORM)" "macos"
  ACCT=pracct
  ACCTFLAGS = -DHAS_AXSIG -DHAS_ACORE
endif
ifeq "$(PLATFORM)" "solaris"
  ACCT=pracct
  ACCTFLAGS = -DHAS_AC_STAT
endif

PROGS =	echoall exec1 exec2 fork1 fork2 nice pruids tellwait1 tellwait2 test1 times1 vfork1 wait1
MOREPROGS = systest1 systest3

all:	$(PROGS) $(MOREPROGS) system.o $(ACCT)

%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

systest1:	system.o systest1.o $(LIBAPUE)
		$(CC) $(CFLAGS) -o systest1 systest1.o system.o $(LDFLAGS) $(LDLIBS)

systest3:	system.o systest3.o $(LIBAPUE)
		$(CC) $(CFLAGS) -o systest3 systest3.o system.o $(LDFLAGS) $(LDLIBS)

pracct:		pracct.c $(LIBAPUE)
		$(CC) $(CFLAGS) $(ACCTFLAGS) -o pracct pracct.c $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ACCT)

include $(ROOT)/Make.libapue.inc
