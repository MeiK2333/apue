#include "apue.h"

int main() {
    char buf[4];

    while (fgets(buf, 4, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            err_sys("output error");
        }
    }

    if (ferror(stdin)) {
        err_sys("input error");
    }
    return 0;
}