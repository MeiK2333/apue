#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int fd = open("sparse_file", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open failed");
        return -1;
    }
    off_t offset = 1024 * 1024; // 1MB
    for(int i = 0; i < 10; i++)
    {
        // 将文件指针向后移动到 1MB 位置（创建空洞）
        if (lseek(fd, offset - 1, SEEK_CUR) == -1) {
            perror("lseek failed");
            close(fd);
            return -1;
        }

        // 写入一个字节，使文件实际大小为 1MB（前 1MB-1 是空洞）
        write(fd, "a", 1);
    }

    close(fd);
    return 0;
}

