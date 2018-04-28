#include "apue.h"

int main() {
    char *ptr;
    size_t size;
    if (chdir("/tmp") < 0) {
        err_sys("chdir failed");
    }
    ptr = path_alloc(&size);
    if (getcwd(ptr, size) == NULL) {
        err_sys("getcwd failed");
    }
    printf("cwd = %s\n", ptr);
    exit(0);
}