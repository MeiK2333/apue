#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: emptyfileCopy file1 file2\n");
        exit(EXIT_FAILURE);
    }

    char buff[BUFF_SIZE];
    int fd1, fd2;

    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if ((fd2 = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int cnt, allCnt = 0;
    int empty2NonemptyFlag = 0;
    int nonEmptyStart = 0;

    while ((cnt = read(fd1, buff, BUFF_SIZE)) > 0) {
        empty2NonemptyFlag = 0;
        nonEmptyStart = 0;
        for (int i=0; i<cnt; i++) {
            if (empty2NonemptyFlag == 0 && buff[i] != 0) {        // 如果在空洞状态(empty2NonemptyFlag:0)遇到非空洞(empty2NonemptyFlag:1)，则记录下来起始位置（nonEmptyStart）， lseek 调整位置
                empty2NonemptyFlag = 1;
                nonEmptyStart = i;
                printf("nonEmptyStart=%d\n", nonEmptyStart);
            } else if (empty2NonemptyFlag == 1 && buff[i] == 0) { // 如果在非空洞状态遇到空洞，则将之前的非空洞序列复制
                if (write(fd2, buff + nonEmptyStart, i - nonEmptyStart) == -1) {
                    perror("write\n");
                    exit(EXIT_FAILURE);
                }
                empty2NonemptyFlag = 0;
                printf("i=%d\n", i);
            }
            
            if (empty2NonemptyFlag == 0) {
                nonEmptyStart = i;
            }
        }

        if (nonEmptyStart != cnt-1) {
            if (write(fd2, buff+nonEmptyStart, cnt - nonEmptyStart) == -1) {
                perror("write\n");
                exit(EXIT_FAILURE);
            }
        }

        allCnt += cnt;
    }

    exit(EXIT_SUCCESS);
}