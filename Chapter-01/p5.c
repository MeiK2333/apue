#include <stdio.h>

const int max = ~(1 << 31);

int main() {
    printf("%f\n", max * 1.0 / 100 / 60 / 60 / 24);
    return 0;
}