
# OSN Assignment 1

##Assumption

We have taken the chunk size to be 1MB(1048576 Bytes). By doing to we're preventing if memory leakage

## Task
Question 1

We have to write the program to reverse the content of the file and store it in a new file having directory named "Assignment".
Ther percentage of the file written should be printed on the console during fle writing and should be overwritten everytime.
The input file path would be given to you as an argument

The directory created should have read, write and execute permissions for the user who created it. The new file created should
have the read and write permissions for the user who created it.

The programs will be tested on large(>1GB) files which could be greater than 
RAM size.

Question 2

Write a program to reverse a specific portion of the file and store the result in a new file in the directory named
“Assignment”.
The percentage of the file written should be printed on the console during file writing. The percentage of the file
written should be overwritten each time (shouldn’t write multiple times).
The directory created should have read, write, and execute permissions for the user who created it. The new file
created should have the read and write permissions for the user who created it.
The program will be tested on LARGE (>1GB) files which could be greater than RAM size.
1
The input file path would be given to you as an argument to your program along with the start and stop 
character, reverse the rest of the file and leave the portion of the file from the start character to the stop character
untouched

## Approach
Question 1:

For reversing the file, first we have to read the file in chunks. We have taken the chunk size of 100MB
. The reason for reading the file in chunks is because the file could have the size
which is greater than the RAM size. So to avoid the memory of RAM being full. We'll
assign our program a 100MB of ram.
We'll first read the file and set our curser at the end of file and read 1MB from the back and store it in an array. After that we'll
reverse it using a pointer from the back and copy it to another array from the front.
Then we'll write that 100MB of data into the desired file (stored in a given directory)

Question 2:

For this problem. We'll break the file into 3 parts. First, from zero to start flag. We'll reverse it then write it into output file.
After that we'll read the file from start to stop flag as it is and write it to output file.
And the third part will be left to reverse and by reversing that part we'll write it again into 
output file.
# Sys-call
