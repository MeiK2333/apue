#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd;
    if ((fd = open("test.txt", O_RDWR | O_APPEND)) == -1) {
        printf("open failed\n");
        exit(0);
    }

    write(fd, "World!\n", 7);

    int r;
    if ((r = lseek(fd, 0, SEEK_SET)) == -1) {
        printf("lseek failed\n");
        close(fd);
        exit(0);
    }

    write(fd, "Hello", 5);
    close(fd);

    if ((fd = open("test.txt", O_RDWR | O_APPEND)) == -1) {
        printf("open failed\n");
        exit(0);
    }

    if ((r = lseek(fd, 0, SEEK_SET)) == -1) {
        printf("lseek failed\n");
        close(fd);
        exit(0);
    }

    char str[32];
    read(fd, str, 15);
    printf("%s\n", str);
    close(fd);

    return 0;
}