#include "apue.h"
#include <sys/utsname.h>

int main() {
    struct utsname us;
    if (uname(&us) == -1) {
        err_sys("uname error");
    }

    printf("sysname  = %s\n", us.sysname);
    printf("nodename = %s\n", us.nodename);
    printf("release  = %s\n", us.release);
    printf("version  = %s\n", us.version);
    printf("machine  = %s\n", us.machine);
    exit(0);
}