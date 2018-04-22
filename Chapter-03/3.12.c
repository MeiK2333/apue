#include <unistd.h>

int main() {
    int fd1 = dup(STDOUT_FILENO);
    int fd2 = dup2(STDOUT_FILENO, STDERR_FILENO);
    write(fd1, "Hello ", 6);
    write(fd2, "World!\n", 7);
    write(STDERR_FILENO, "你好世界\n", 13);
    return 0;
}