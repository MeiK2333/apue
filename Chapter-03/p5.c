#include <unistd.h>

int main() {
    write(STDOUT_FILENO, "Hello ", 7);
    write(STDERR_FILENO, "World!\n", 9);
    return 0;
}