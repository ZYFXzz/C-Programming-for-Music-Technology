#include <stdio.h>
#include <string.h>

int my_atoi(char *s)
{
    int n = 0;
    // your code here
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        n = n * 10 + (s[i] - '0'); // after searching "0", is the referencing zero point in ASCII system, so subtract it in this way
    }
    /*
    also possible to do it in the reverse direction
    like from the last digit and go up to the digits in the front
    */
    // int m = 1;
    // for (int i = len - 1; i >= 0; i--)
    // {
    //     n += (s[i] - '0') * m;
    //     m *= 10;
    // }

    return n;
}

int main()
{
    char *s1 = "153";
    char *s2 = "23";
    printf("The sum of %s and %s is %i\n", s1, s2, my_atoi(s1) + my_atoi(s2));
    return 0;
}