#include <stdio.h> 
#include <stdlib.h>
#include "head.h"


//Linear Search
// int main(int argc, char** argv){

//     const int seq_length = 10;
//     int sequence[seq_length] = {3,7,1,5,9,76,2,234,-3,-23};

//     int target = atoi(argv[1]);

//     int found_index = -1;

//     for( int i = 0; i < seq_length; i ++){

//         if(target == sequence[i]){
//             found_index = i;
//         }
//     }

//     printf("Index is %d!!\n", found_index);

//     return 0;
// }






//Binary Search
int binary_search(int *arr, int low, int high, int target){
    while(low <= high){

        int mid = (low + high) / 2;

        if(target == arr[mid]){
            return mid;
        }

        if( arr[mid] < target){
            low = mid + 1;
        }

        if(arr[mid] > target){
            high = mid -1;
        }

    }

    return -1;
    
    // if (low > high){
    //     return -1;
    // }

    // int mid = (low + high) / 2;
    
    

    // if(target == arr[mid]){
    //     printf("mid = %d\n", mid);
    //     return mid;
    // }
    
    // if( arr[mid] < target){
    //     binary_search(arr, mid+1, high, target);
    // }

    // if(arr[mid] > target){
    //     binary_search(arr, low, mid-1, target);
    // }

}

// int main(int argc, char** argv){

//     const int seq_length = 10;
//     int sequence[seq_length] = {-23,-3, 3,7,8,10,30,76,102,234};

//     int target = atoi(argv[1]);

//     int found_index = binary_search(sequence, 0, seq_length-1, target);

//     printf("Index is %d!!\n", found_index);

//     return 0;
// }











//Hash Search
















//Linear Search
// int search(int arr[], int N, int x)
// {
//     for (int i = 0; i < N; i++)
//         if (arr[i] == x)
//             return i;
//     return -1;
// }

// Driver code
// int main(void)
// {
//     int arr[] = { 2, 3, 4, 10, 40 };
//     int x = 10;
//     int N = sizeof(arr) / sizeof(arr[0]);

//     // Function call
//     int result = search(arr, N, x);
//     (result == -1)
//         ? printf("Element is not present in array")
//         : printf("Element is present at index %d", result);
//     return 0;
// }


//Binary Search How to make this recursive
// int binarySearch(int arr[], int low, int high, int target) {
//     while (low <= high) {
//         int mid = low + (high - low) / 2;

//         if (arr[mid] == target) {
//             return mid;
//         } else if (arr[mid] < target) {
//             low = mid + 1;
//         } else {
//             high = mid - 1;
//         }
//     }
//     return -1;
// }

// int main() {
//     int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
//     int n = sizeof(arr) / sizeof(arr[0]);
//     int target = 23;

//     int result = binarySearch(arr, 0, n - 1, target);

//     if (result == -1) {
//         printf("Element is not present in array\n");
//     } else {
//         printf("Element is present at index %d\n", result);
//     }
//     return 0;
// }

//Hash Search


