#include <stdio.h>
#include <string.h>
#define DLEN 32
#define _FORTIFY_SOURCE 0

int main(void)
{
    char dest[DLEN];
    strcpy(dest, "C programming");
    printf("dest: %s\n", dest);
    strcat(dest, " is really, really fun");
    printf("dest: %s\n", dest);
    return 0;
}