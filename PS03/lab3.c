#include <stdio.h>
#include <stdlib.h>

// int main(int argc, char **argv)
// {

//     if (argc != 3)
//     {
//         printf("please input current number of elements");
//         return 1;
//     }

//     int x = atoi(argv[1]);
//     int y = atoi(argv[2]);
//     printf("the x is %d", x);
//     printf("the y is %d", y);
//     if (x > y)
//     {
//         printf("the first greater than the second\n");
//     }
//     else if (x < y)
//     {
//         printf("the first is smaller than the second\n");
//     }
//     else
//     {
//         printf("the first equal to the second");
//     }
// }

// int main()
// {

//     char a = 'b';

//     while (1)
//     {
//         printf("input a character:");
//         scanf('%c', &a);

//         if (a == 'z')
//         {
//             break;
//         }
//     }
//     return 0;
// }

int main(int argc, char **argv)
{

    char var = argv[1][0];

    switch (var)
    {
    case 'a':
        printf("case a is matched");
        break;

    case 'b':
        printf("case b is matched");
        break;
    case 'c':
        printf("case c is matched");
        break;
    default:
        printf("default case a is matched");
        break;
    }

    return 0;
}