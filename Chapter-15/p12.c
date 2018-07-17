#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

int main() {
    int i, id;
    char *hello = "Hello World!\n";
    for (i = 0; i < 5; i++) {
        if ((id = msgget(0, IPC_CREAT)) == -1) {
            printf("msgget error\n");
            exit(1);
        }
        printf("id = %d\n", id);
        if (msgctl(id, IPC_RMID, NULL) == -1) {
            printf("msgctl IPC_RMID error\n");
            exit(1);
        }
    }

    printf("\n\n");

    for (i = 0; i < 5; i++) {
        if ((id = msgget(IPC_PRIVATE, 0)) == -1) {
            printf("msgget error\n");
            exit(1);
        }
        printf("id = %d\n", id);
        if (msgsnd(id, hello, 0, IPC_NOWAIT) == -1) {
            printf("%s\n", strerror(errno));
            printf("msgsnd error\n");
            exit(1);
        }
    }
    exit(0);
}