#include <stdio.h>
int pascal[16][16];

int main()
{
    // calculation of the triangle
    for (int i = 0; i < 16; i++)
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
            printf("%5d", pascal[i][j]); 
        }
        printf("\n"); 
    }
    return 0;
}