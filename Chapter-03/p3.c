#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
    printf("-------------------------------------------------\n");
    int fd1 = open("test.txt", O_RDONLY);
    int fd2 = dup(fd1);
    int fd3 = open("test.txt", O_RDONLY);

    printf("fd1 = %d\n", fd1);
    printf("fd2 = %d\n", fd2);
    printf("fd3 = %d\n", fd3);
    printf("-------------------------------------------------\n");

    int fd1_flags = fcntl(fd1, F_GETFD);
    int fd2_flags = fcntl(fd2, F_GETFD);
    int fd3_flags = fcntl(fd3, F_GETFD);

    printf("Before fcntl(fd1, F_SETFD, fd1_flags):\n");
    printf("fd1_flags = %d\n", fd1_flags);
    printf("fd2_flags = %d\n", fd2_flags);
    printf("fd3_flags = %d\n", fd3_flags);
    printf("-------------------------------------------------\n");

    // 设置 FD_CLOEXEC 标志（其他标志不变）
    fd1_flags |= FD_CLOEXEC;
    fcntl(fd1, F_SETFD, fd1_flags);

    printf("After fcntl(fd1, F_SETFD, fd1_flags):\n");
    printf("fd1_flags = %d\n", fd1_flags);
    printf("fd2_flags = %d\n", fd2_flags);
    printf("fd3_flags = %d\n", fd3_flags);
    printf("-------------------------------------------------\n");

    printf("Before fcntl(fd1, F_SETFL, fd1_file_status_flags):\n");
    printf("fd1_file_status_flags = %d\n", fcntl(fd1, F_GETFL, 0));
    printf("fd2_file_status_flags = %d\n", fcntl(fd2, F_GETFL, 0));
    printf("fd3_file_status_flags = %d\n", fcntl(fd3, F_GETFL, 0));
    printf("-------------------------------------------------\n");

    int fd1_file_status_flag = fcntl(fd1, F_GETFL, 0);
    fd1_file_status_flag |= O_APPEND;
    fcntl(fd1, F_SETFL, fd1_file_status_flag);

    printf("After fcntl(fd1, F_SETFL, fd1_file_status_flags):\n");
    printf("fd1_file_status_flags = %d\n", fcntl(fd1, F_GETFL, 0));
    printf("fd2_file_status_flags = %d\n", fcntl(fd2, F_GETFL, 0));
    printf("fd3_file_status_flags = %d\n", fcntl(fd3, F_GETFL, 0));
    printf("-------------------------------------------------\n");

    return 0;
}
