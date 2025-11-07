#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct node_s{
    int fd;
    struct node_s* next;
}Node;

void free_fd(Node* head)
{
    Node* cur = head;
    while(cur != NULL)
    {
        Node* temp = cur;
        cur = cur->next;
        if(temp->fd != -1)
        {
            close(temp->fd);
            printf("Close fd %d\n", temp->fd);
        }
        free(temp);
    }
}

int my_dup2(int oldfd, int newfd)
{
    if(newfd == oldfd)
        return oldfd;

    close(newfd);

    Node* head = (Node*)malloc(sizeof(Node));
    head->fd = -1;
    head->next = NULL;
    Node* cur = head;

    int fd_temp;

    while((fd_temp = dup(oldfd)) != newfd)
    {
        if(fd_temp == -1)
        {
            free_fd(head);
            return -1;
        }

        printf("Create fd %d\n", fd_temp);
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->fd = fd_temp;
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }
    free_fd(head);
    return newfd;
}


int main()
{
    int newfd = 9;
    /* int fd = dup2(STDOUT_FILENO, newfd); */
    int fd = my_dup2(STDOUT_FILENO, newfd);
    write(fd, "hello world!\n", 13);

    return 0;
}
