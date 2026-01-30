#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 4096

int main(int argc, char* argv[])
{
    if(argc < 3){
        printf("Usage: %s source_file dest_file\n", argv[0]);
        exit(1);
    }

    int fd1, fd2;
    char buf[BUF_SIZE];

    if((fd1 = open(argv[1], O_RDONLY)) == -1){
        perror("open source file");
        exit(1);
    }

    if((fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1){
        perror("open dest file");
        exit(1);
    }

    ssize_t nread;
    off_t file_pos = 0;      // 源文件中的当前位置
    int in_hole = 1;         // 初始状态为空洞（文件开头可能是空洞）
    off_t data_start = 0;    // 当前数据块的开始位置
    off_t hole_start = 0;    // 当前空洞的开始位置

    while((nread = read(fd1, buf, BUF_SIZE)) > 0){
        for(int i = 0; i < nread; i++){
            if(buf[i] != 0 && in_hole){
                // 非0字节 - 数据部分，从空洞切换到数据
                in_hole = 0;
                data_start = file_pos + i;
                // 移动到数据开始位置
                if(lseek(fd2, data_start, SEEK_SET) == -1){
                    perror("lseek");
                    exit(1);
                }
            }
            if(buf[i] == 0 && !in_hole)
            {
                // 0字节 - 空洞部分，从数据切换到空洞
                in_hole = 1;
                hole_start = file_pos + i;
                // 写入之前的数据块
                if(write(fd2, buf + (data_start - file_pos),
                            hole_start - data_start) == -1){
                    perror("write");
                    exit(1);
                }
            }
        }
        file_pos += nread;
    }

    // 处理文件末尾可能的数据块
    if(!in_hole){
        // 文件以数据结束
        if(write(fd2, buf + (data_start - (file_pos - nread)),
                    file_pos - data_start) == -1){
            perror("write");
            exit(1);
        }
    }

    // 确保目标文件大小正确
    if(ftruncate(fd2, file_pos) == -1){
        perror("ftruncate");
        exit(1);
    }

    close(fd1);
    close(fd2);
    return 0;
}
