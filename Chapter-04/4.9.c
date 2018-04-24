#include "apue.h"

int main() {
    struct stat statbuf;

    if (stat("foo", &statbuf) < 0) {
        err_sys("stat error for foo");
    }

    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        err_sys("chmod error for foo");
    }

    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
        err_sys("chmod error for bar");
    }

    exit(0);
}