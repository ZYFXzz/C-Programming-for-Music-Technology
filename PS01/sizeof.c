#include <stdio.h>
#include <stdbool.h>

int main(void)
{

    // char
    printf("size of char: %zu\n", sizeof(char));
    // int
    printf("size of int: %zu\n", sizeof(int));
    // short
    printf("size of short: %zu\n", sizeof(short));
    // long
    printf("size of long: %zu\n", sizeof(long));
    // long long
    printf("size of long long: %zu\n", sizeof(long long));

    //

    // bool
    printf("size of bool: %zu\n", sizeof(bool));
    // unsigned int
    printf("size of unsigned int: %zu\n", sizeof(unsigned int));
    // float
    printf("size of float: %zu\n", sizeof(float));
    // double
    printf("size of double: %zu\n", sizeof(double));
    // long double
    printf("size of long double: %zu\n", sizeof(long double));
    // 12345
    printf("size of 12345: %zu\n", sizeof(12345));
    // 123456789012
    printf("size of 123456789012: %zu\n", sizeof(123456789012));
    // 3.14
    printf("size of 3.14: %zu\n", sizeof(3.14));
    // 3.14f
    printf("size of 3.14f: %zu\n", sizeof(3.14f));
    // hello
    printf("size of hello: %zu\n", sizeof("hello"));

    return 0;
}