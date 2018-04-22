#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
    int fd1 = open("test.txt", O_RDONLY);
    int fd2 = dup(fd1);
    int fd3 = open("test.txt", O_RDONLY);

    printf("fd1 = %d\n", fcntl(fd1, F_GETFD));
    fcntl(fd1, F_SETFD, 1);
    printf("fd1 = %d\n", fcntl(fd1, F_GETFD));

    printf("fd2 = %d\n", fcntl(fd2, F_GETFD));
    fcntl(fd1, F_SETFD, 0);

    fcntl(fd1, F_SETFL, 3);
    printf("fd1 = %d\n", fcntl(fd1, F_GETFL));
    printf("fd2 = %d\n", fcntl(fd2, F_GETFL));
    printf("fd3 = %d\n", fcntl(fd3, F_GETFL));
    return 0;
}