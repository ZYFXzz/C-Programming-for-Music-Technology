#include <stdio.h> 
#include <stdlib.h>

// int sum(int k) {
//   if (k > 0) {
//     return k + sum(k - 1);
//   } else {
//     return 0;
//   }
// }

// void recursion(){
//     int result = sum(2);
//     printf("%d", result);
// }

// void switch_test(){
//         // Switch variable
//     int var = 1;

//     // Switch statement
//     switch (var) {
//     case 1:
//         printf("Case 1 is Matched.");
//         break;

//     case 2:
//         printf("Case 2 is Matched.");
//         break;

//     case 3:
//         printf("Case 3 is Matched.");
//         break;

//     default:
//         printf("Default case is Matched.");
//         break;
//     }
// }


// int main(int argc, char *argv[]) {
//     recursion();
//     switch_test();
//     printf("%f", atof(argv[1]));
//     return 0;
// }














//If else test: print different statements depending on  how argv[1] compares to argv[2]
// int main(int argc, char** argv ){

//     if(argc != 3){
//         printf("PLEASE INPUT THE CORRECT AMOUNT OF ARGUEMENTS!!");
//         return 1;
//     }

//     int x = atoi(argv[1]);
//     int y = atoi(argv[2]);

//     if( x > y){
//         printf("The first number is greater than the second\n");
//     }
//     else if(x < y){
//         printf("The first number is not greater than the second\n");
//     }
//     else{
//         printf("The first number is equal to the second\n");
//     }

//     return 0;
// }

//For loop test: print every element in the argv input
// int main(int argc, char**argv){

//     for(int i = 0; i < argc; i++){
//         printf("%s\n", argv[i]);
//     }

//     return 0;
// }

//while loop test: Continue to prompt the user for a value until they give you the correct character
// int main(){

//     char a = 'b';

//     while(1){
//         printf("Input a character: ");
//         scanf("%c", &a);

//         if(a == 'z'){
//             break;
//         }
        
//     }

//     return 0;
// }
//switch test: print different phrases depening on the character input
int main(int argc, char ** argv){

    char var = argv[1][0];

    switch (var) {
    case 'a':
        printf("Case a is Matched.");
        break;

    case 'b':
        printf("Case b is Matched.");
        break;

    case 'c':
        printf("Case c is Matched.");
        break;

    default:
        printf("Default case is Matched.");
        break;
    }
}

//recursion test: recursively add numbers until you get z  (add n + (n-1) + (n-2) ...  + 0)