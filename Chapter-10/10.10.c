#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_alrm(int signo) {

}

unsigned int sleep1(unsigned int seconds) {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        return (seconds);
    }
    alarm(seconds);
    pause();
    return (alarm(0));
}

int main() {
    printf("before sleep\n");
    sleep1(3);
    printf("after sleep\n");
    return 0;
}