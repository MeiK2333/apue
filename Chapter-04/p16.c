#include "apue.h"
#include <fcntl.h>

const int depth = 1000;


int main(int argc, char *argv[]) {
    int i;
    size_t size;
    char *path;

    int path_max = sysconf(_PC_NAME_MAX);
    char *path_name;
    path_name = (char *) malloc ((path_max + 1) * sizeof(char));
    for (i = 0; i <= path_max; i++) {
        path_name[i] = '-';
    }
    
    for (i = 0; i < depth; i++) {
        if (mkdir(path_name, DIR_MODE) < 0) {
            printf("mkdir failure\n");
            exit(1);
        }
        if (chdir(path_name) < 0){
            printf("chdir failure\n");
            exit(1);
        }
        path = path_alloc(&size);
        char *this_name = getcwd(path, &size);
        printf("%s\n", this_name);
    }
    return 0;
}