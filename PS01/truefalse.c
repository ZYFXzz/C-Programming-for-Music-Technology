#include <stdio.h>
int main()
{

    printf("2: % s\n", 2 ? "true" : "false");
    printf("3: % s\n", 3 ? "true" : "false");
    printf("3.14: % s\n", 3.14 ? "true" : "false");
    printf("0.0: % s\n", 0.0 ? "true" : "false");
    printf("0.0f: % s\n", 0.0f ? "true" : "false");
    printf("hello: % s\n", "hello" ? "true" : "false");

    return 0;
}
