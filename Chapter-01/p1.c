#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


void checkLs() {
    DIR *dp1, *dp2;
    struct dirent *dirp1, *dirp2;

    char *dir1 = ".";
    char *dir2 = "..";

    if ((dp1 = opendir(dir1)) == NULL)
        printf("can't open %s", dir1);

    if ((dp2 = opendir(dir2)) == NULL)
        printf("can't open %s", dir2);

    while ((dirp1 = readdir(dp1)) != NULL && (dirp2 = readdir(dp2)) != NULL)
        if (strcmp(dirp1->d_name, dirp2->d_name) != 0) {
            printf("%s 与 %s 目录内文件不同\n", dir1, dir2);
            closedir(dp1);
            closedir(dp2);
            return;
        }

    printf("%s 与 %s 目录内文件相同\n", dir1, dir2);
    closedir(dp1);
    closedir(dp2);
}


void checkPwd() {
    char path1[1024], path2[1024];
    realpath(".", path1);
    realpath("..", path2);
    if (strcmp(path1, path2) == 0) {
        printf("%s 与 %s 的绝对路径相同\n", path1, path2);
    } else {
        printf("%s 与 %s 的绝对路径不同\n", path1, path2);
    }
}


int main(int argc, char *argv[]) {
    checkLs();
    checkPwd();
    exit(0);
}