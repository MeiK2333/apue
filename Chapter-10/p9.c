#include "apue.h"
#include <error.h>

void pr_mask(const char *str) {
    sigset_t sigset;

    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_ret("sigprocmask error");
    } else {
        printf("%s", str);
        if (sigismember(&sigset, SIGINT)) {
            printf(" SIGINT");
        }
        /* 此处省略很多条 */
        printf("\n");
    }
}

int main() {
    return 0;
}