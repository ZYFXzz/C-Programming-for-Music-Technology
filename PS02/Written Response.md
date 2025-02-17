# Responses for Set 2

## Problem 1: lexical scoping
scope1.c, scope2.c
**Question 1:** 
**terminal output:**

1  
2  
3  
4  
5  
6  
7  
8  
9  
10  
11  

**Response:** 
- ***a*** 11
- ***b*** The for loop iterates from i = 1 to i = 10. In each iteration, the current value of i is printed. After the final iteration (when i = 10), the loop condition i <= 10 is checked again.  
At this point, i is incremented to 11, which no longer satisfies the loop condition (11 <= 10 is false), causing the loop to terminate.  
After the loop terminates, the printf statement outside the loop is executed, printing the current value of i, which is now 11. Therefore, the last line printed is 11. 

**"Question 2:"**
**terminal output:**  
1  
2  
3  
4  
5  
6  
7  
8  
9  
10  
0  
- ***a***  0
- ***b***  it is different.   
i within the for loop statement is a local variable only, it will only exist within the for loop execution, and will be deprecated after for loop completion, the other int i on line 5, there is a place in the memory allocated for this variable, but without any value assigned to it.   
Therefore, the printf will print 0, as the default initialized value for that i. 

Inserting the int in the loop initialization will create a new variable for this loop.
## Problem 2: String Operations and Buffer Overrun
buf1.c, buf2.c

**Question:** 
2.a What happens when you execute buf2.c?
2.b When you concatenate the second string and the first, what it the length of the new string? How does that compare to the length of the dest[] array?


**Response:** 
- ***a***  
it prints the message "C programming is really, really fun"
- ***b***  the length is 35 for the new string. It is longer than the dest[] array which have a length of 32.


## Problem 3: ASCII value

code implementation only


## Problem 4:  Pascal's Triangle 


  
**Question:**   

2 Program pascal2.c. Instead of printing numbers, modify your program to print "*"
if the number is odd, and " ", a space, if even. Print row 0 to 31.
2.a What patterns do you see?


**Response:** 
The pattern consists of groups of triangles stacked together in a recursive manner.  
Initially, small triangle units form at the lowest levels. As the rows increase, three adjacent small triangles combine to form a larger triangle.  
This process repeats at multiple scales, with each newly formed larger triangle acting as a building block for an even bigger structure.  
This repetition continues until the final row, creating a fractal-like pattern.


