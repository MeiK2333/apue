#include <stdio.h>

int main() {
    int a = 0x04030201;
    printf("%d\n", a);
    char *ch;
    ch = (char *)(&a);
    printf("ch0 = %d\n", ch[0]);
    printf("ch1 = %d\n", ch[1]);
    printf("ch2 = %d\n", ch[2]);
    printf("ch3 = %d\n", ch[3]);
    return 0;
}