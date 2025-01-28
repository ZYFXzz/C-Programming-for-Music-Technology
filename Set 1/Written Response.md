# Responses for Set 1

## Problem 1: printf vs puts
hello.c
**Question:** 

An alternative to printf in your hello_world.c program is puts("Hello, World!"). 
Write a program hello.c that is  a new “Hello, World!” program using puts(). 
What are the two main differences between printf and puts? For these reasons, puts() is more 
appropriate for printing static messages.

**Response:** 
- ***printf()*** printf() supports formatted outputs, allows "grouping" different types of inputs together, like putting static strings and variables together. 
- ***puts()*** puts() is only print static strings, and will automatically append a newline (\n) at the end, and printf() need explicitly adding it.

For printing static string messages like "Hello, World!", puts() is more appropriate because it is faster and simpler.

## Problem 2: sizeof
sizeof.c  

**Question:** 

Using sizeof(), we can know the size (in bytes) of a type or a value. It evaluates to a number in 
size_t type, which is an unsigned integer but whose size is platform-dependent. 
You can print the number using printf() and format string "%zu\n", for example: 
printf("sizeof char: %zu\n", sizeof(char)); 
Write a program sizeof.c printing the sizes of the following types and values.  

**Print Outputs:**

size of char: 1  
size of int: 4  
size of short: 2  
size of long: 4  
size of long long: 8  
size of bool: 1  
size of unsigned int: 4  
size of float: 4  
size of double: 8  
size of long double: 12  
size of 12345: 4  
size of 123456789012: 8  
size of 3.14: 8  
size of 3.14f: 4  
size of hello: 6  

a. Why are the sizes of 12345 and 123456789012 different?  
b. Why are the sizes of 3.14 and 3.14f different?  
c. What does the size of "hello" mean? If not sure, try with different strings.  
d. Why would bool need that many bytes, where one bit is sufficient to represent true/false.  

**Response:** 
- ***a***  
12345 can be hold by an int or a long, 123456789012 is larger than the number range that can be represented by int or long.       
Therefore, to represent 123456789012, we need long long that uses 8 bytes space on the machine.
- ***b***  
3.14 is a double, which is 8 bytes. 3.14f explicitly denotes a float, only 4 bytes.
- ***c***  
The message contains 5 characters (h, e, l, l, o) and a null terminator (\0) at the end, each component make up 1 byte space. 6 bytes in total
- ***d***  
Although, in theory, we can express 0/1 or true/false using just 1 bit, in practice, we require additional context provided through extra bits. This is necessary to interpret the data, similar to how we differentiate between signed int and unsigned int. In the case of integers, the first bit is used to indicate the + or - sign, rather than contributing to the numeric value itself.  
Another technical reason why bool occupies 1 byte instead of 1 bit is due to how modern computer architectures are designed. Most processors handle memory in units of 1 byte (8 bits) as the smallest processing unit. Processing individual bits would make it less efficient. 

## Problem 3: floating point accuracy 
float.c, double.c 


Create a program float.c that prints a float value with 20 decimal places using format string 
"%.20f\n".    

**Print Outputs:**

- double. c  
1.00000000000000000000
0.50000000000000000000
0.10000000000000001000
- float. c  
1.00000000000000000000
0.50000000000000000000
0.10000000149011612000  

**Question:**   

Using this, print the numbers 1.0, 0.5, and 0.1. Which of the printed numbers is 
inaccurate?  
Why would such a simple number like this be stored inaccurately in computer while 
the others are not?  
Create a second program double.c in which the variables are declared as 
double. What happens now?  


**Response:** 
- ***which is in accurate:***   
0.1
- ***why would this stored inaccurately*** 
Since computer architecture are designed with binary systems, numbers like 1.0 and 0.5 can be perfectly represented by binary digits, whereas 0.1 cannot.

- ***what happens in double.c***
0.1 representation in double is still inaccurate, but much more closer to the 0.1 value than the ones declared in float.  
The type double uses more bytes to represent numbers, doubling the decimal digits almost doubles the precision.

## Problem 4:  nonzero values
truefalse.c


We know that 0 is evaluated as false, and 1 is evaluated as true. Write a program truefalse.c 
to find out how the following values are evaluated when used as the condition of an if statement.    

**Print Outputs:**
2: true  
3: true  
3.14: true  
0.0: false  
0.0f: false  
hello: true  

**Question:**   

What is the criteria for evaluating an if condition as true?   


**Response:** 
In general, any non-zero values are considered as true.  
The only zero values listed here are 0.0, and 0.0f.   
Strings like "hello" also is non-zero values, so if evaluation returns true.
