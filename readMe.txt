Name: Benjamin Pham
ID: 002106181
Language: C
Project Name: Sudoku.c
Compile command for Sudoku.c: ggc -pthread -o Sudoku Sudoku.c

Description:
This program is a C program that takes in a 9x9 sudoku file input as a parameter in the program and prints out if the 
sudoku file is valid or invalid. An invalid sudoku will print out the threads that are invalid meaning they contain
more, less, or same values in each line of the file. 27 child threads will be created from the 9x9 sudoku txt file. 
A parent thread will make it a total of 28 threads but only the children threads will be looked at. Threads are created
by using structs and parameters placed into each functions associating with the conditions in the if statements. 

Installing:
Use any IDE/C compilers and open the file on those environments.

How to use:
In the command line, first compile the files with compile command above for the respective files. 
Then run the C compiled file by typing ./Sudoku and then once the prompt is given to the user to 
type in a file (should be in the same directory for easy access), the user should type in the file name
with the extension too.

Credit:
This program was first sampled by TAs from GSU cs class Operating Systems CSC 4320 lectured by Dr. Yuan Li.
The program was completed by Benjamin Pham. 