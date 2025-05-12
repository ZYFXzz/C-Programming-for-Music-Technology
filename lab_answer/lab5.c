#include <stdio.h> 
#include <stdlib.h>
#include "head.h"

//O(n) questions

//Bubble Sort

int main(){

    int N = 10;
    int arr[N] = {4,8,1,9,2,5,34,9,2,3};

    for(int i =0; i < N-1; i ++)
    {
        for(int j = 0; j <N-1-i; j++)
        {
            if(arr[j] > arr[j+1])
            {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    for(int i =0; i < 10; i ++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}

//O(N) questions

// int a = 0, b = 0;
// for (i = 0; i < N; i++) {
//     a = a + rand();
// }
// for (j = 0; j < M; j++) {
//     b = b + rand();
// }

// int a = 0;
// for (i = 0; i < N; i++) {
//     for (j = N; j > i; j--) {
//         a = a + i + j;
//     }
// }

// int i, j, k = 0;
// for (i = n / 2; i <= n; i++) {
//     for (j = 2; j <= n; j = j * 2) {
//         k = k + n / 2;
//     }
// }

//Bubble Sort
// void swap(int* xp, int* yp){
//     int temp = *xp;
//     *xp = *yp;
//     *yp = temp;
// }

// // An optimized version of Bubble Sort
// void bubbleSort(int arr[], int n){
//     int i, j;
//     int swapped;
//     for (i = 0; i < n - 1; i++) {
//         swapped = 0;
//         for (j = 0; j < n - i - 1; j++) {
//             if (arr[j] > arr[j + 1]) {
//                 swap(&arr[j], &arr[j + 1]);
//                 swapped = 1;
//             }
//         }

//         // If no two elements were swapped by inner loop,
//         // then break
//         if (swapped == 0 )
//             break;
//     }
// }

// // Function to print an array
// void printArray(int arr[], int size){
//     int i;
//     for (i = 0; i < size; i++)
//         printf("%d ", arr[i]);
// }

// int main(){
//     int arr[] = { 64, 34, 25, 12, 22, 11, 90 };
//     int n = sizeof(arr) / sizeof(arr[0]);
//     bubbleSort(arr, n);
//     printf("Sorted array: \n");
//     printArray(arr, n);
//     return 0;
// }