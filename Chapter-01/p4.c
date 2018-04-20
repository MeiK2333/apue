#include <stdio.h>

const int max = ~(1 << 31);

int main() {
    printf("%f\n", max * 1.0 / 60 / 60 / 24 / 365);
    return 0;
}