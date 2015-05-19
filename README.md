# Solutions for COP3503 (C++)

## About
This repository is dedicated to providing the solutions for the assignments to COP3503 Spring 2015 session at the University of Florida. Since this course is Programming Fundamentals II, all solutions will be provided in C++. Please refer to the table of contents below to get a better overview of the modules included in this repository.

## Module Overview
#### [Magic Square](Magic Square/pa1.cpp) 
- [<b>Requirements</b>](Magic Square/Assignment Requirements.pdf)
- <b>Overview: </b> Write a C++ program that creates an N*N magic square, i.e., a square arrangement of the numbers 1, 2, , , in which the sum of rows, columns, and diagonals are the same. The user (i.e., TAs) will specify the size of the square matrix: N. The value N must be an odd number between 3 and 15.

#### [Memory Management Model](Memory Management Model/pa2.cpp) 
- [<b>Requirements</b>](Memory Management Model/Assignment Requirements.pdf)
- <b>Overview: </b> Write a C++ program that emulate the operating system’s responsibility of allocating memory to certain programs. This will be a very simple page-based view of memory management. On startup, your program will have some 32 pages of contiguous, unused memory. Each page will be 4 KB long. It should then allow the users (i.e., your TAs) to “run” programs that require chunks of this memory for some period of time. It should also allow the users to “kill” programs (i.e., “Ctrl-c” or “kill -9” in most OSs) that are in progress. The pages used by programs that are killed can then be re-used for future programs.

#### [Lexical Analysis](Lexical Analysis/pa3.cpp) 
- [<b>Requirements</b>](Lexical Analysis/Assignment Requirements.pdf)
- <b>Overview: </b> In this programming language, you should implement the lexical analysis task for the limited version (i.e., the depth of the nested loops) of a programming language. Lexical analysis is the first stage that compilers parse and detect the possible syntax errors. Ideally, any new (programming) languages can be designed and analyzed in the similar manner. You will need to analyze a Pascal-and-C-like language in this programming assignment.
