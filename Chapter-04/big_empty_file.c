#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * 创建空洞文件示例。
 */
int main(int argc, char **argv) 
{
    int fd = 0;
    off_t location = 0;

    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <fileName> \r\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    write(fd, "begin", 5);
    location = lseek(fd, 512, SEEK_SET);
    if (location == -1)
    {
        perror("lseek()");
        exit(EXIT_FAILURE);
    }

    write(fd, "mid", 3);

    location = lseek(fd, 512, SEEK_CUR);

    write(fd, "end", 3);
    write(fd, "", 1);

    close(fd);

    exit(EXIT_SUCCESS);
}