#include <stdio.h>
int pascal[31][31];
/*
is it possible to use a variable to define the dimension of pascal rows
like
int n = 16;
int pascal[n][n]
*/

int main()
{
    // calculation of the triangle
    for (int i = 0; i < 31; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (j == 0 || j == i) // First and last elements of each row

                pascal[i][j] = 1;
            else
                pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }

    // print
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            if (pascal[i][j] % 2 != 0)
                printf("* ");
            else
                printf("  ");
        }
        printf("\n"); // switch row
    }
    return 0;
}