#include "apue.h"

int main() {
    if (chdir("/tmp") < 0) {
        err_sys("chdir failed");
    }
    printf("chdir to /tmp succeeded\n");
    exit(0);
}