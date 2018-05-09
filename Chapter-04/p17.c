#include "apue.h"

int main(int argc, char *argv[]) {
    int fd;
    if (unlink(argv[1]) == -1) {
        err_sys("unlink failure");
    }
    if ((fd = creat(argv[1], FILE_MODE)) < 0) {
        err_sys("...");
    }
    return 0;
}