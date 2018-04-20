#include <stdio.h>
#include <limits.h>

int main() {
    printf("CHAR_BIT: %d\n", CHAR_BIT);
    printf("CHAR_MAX: %d\n", CHAR_MAX);
    printf("CHAR_MIN: %d\n", CHAR_MIN);
    printf("SCHAR_MAX: %d\n", SCHAR_MAX);
    printf("SCHAR_MIN: %d\n", SCHAR_MIN);
    printf("INT_MAX: %d\n", INT_MAX);
    printf("INT_MIN: %d\n", INT_MIN);
    printf("UINT_MAX: %d\n", UINT_MAX);
    printf("SHRT_MAX: %d\n", SHRT_MAX);
    printf("SHRT_MIN: %d\n", SHRT_MIN);
    printf("USHRT_MAX: %d\n", USHRT_MAX);
    printf("LONG_MAX: %ld\n", LONG_MAX);
    printf("LONG_MIN: %ld\n", LONG_MIN);
    printf("ULONG_MAX: %lu\n", ULONG_MAX);
    printf("LLONG_MAX: %lld\n", LLONG_MAX);
    printf("LLONG_MIN: %lld\n", LLONG_MIN);
    printf("ULLONG_MAX: %llu\n", ULLONG_MAX);
    printf("MB_LEN_MAX: %d\n", MB_LEN_MAX);
    return 0;
}