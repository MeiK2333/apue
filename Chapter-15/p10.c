#include "apue.h"
#include <fcntl.h>

#define FIFO "temp.fifo"

int main() {
    int fdread, fdwrite;

    unlink(FIFO);
    if (mkfifo(FIFO, FILE_MODE) < 0) {
        err_sys("mkfifo error");
    }
    if ((fdread = open(FIFO, O_RDONLY | O_NONBLOCK)) < 0) {
        err_sys("open error for reading");
    }
    if ((fdwrite = open(FIFO, O_WRONLY)) < 0) {
        err_sys("open error for writing");
    }
    clr_fl(fdread, O_NONBLOCK);
    exit(0);
}