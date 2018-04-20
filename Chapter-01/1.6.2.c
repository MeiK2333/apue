#include "apue.h"

int main(int argc, char *argv[]) {
    printf("hello world from process ID %ld\n", (long)getpid());
    exit(0);
}