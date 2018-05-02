#include "apue.h"
#ifdef SOLARIS
#include <sys/mkdev.h>
#endif
#define MINORBITS        20
#define MINORMASK        ((1U << MINORBITS) - 1)
#define MAJOR(dev)        ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)        ((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)      (((ma) << MINORBITS) | (mi))

int main(int argc, char *argv[]) {
    int i;
    struct stat buf;
    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (stat(argv[i], &buf) < 0) {
            err_ret("stat error");
            continue;
        }
        printf("dev = %d/%d", MAJOR(buf.st_dev), MINOR(buf.st_dev));
        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
            printf(" (%s) rdev = %d/%d", (S_ISCHR(buf.st_mode)) ? "character" : "block", MAJOR(buf.st_rdev), MINOR(buf.st_rdev));
        }
        printf("\n");
    }
    exit(0);
}