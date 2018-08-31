# announcement
This application computes the solution to a shared belief revision announcement problem given a set of agents with initial beliefs, and their respective goals.

## Usage:
`./bin/announcement` will launch the GUI
`./bin/announcement -h` will display command-line argument options
`./bin/announcement -t n` will generate a problem with n variables and no possible solution. This is useful for benchmarking the scaling of the tool.
`./bin/announcement -b` will perform a full brute force of the 2-bit variable input space using 2 agents. This can also be used for benchmarking, but also for testing correctness.

## Input formats:
Input data is entered manually through formula strings

A formula consists of the following tokens:
Any positive decimal integer representing a variable - Floating point and negative values, as well as zero are not allowed
The following operations are permitted: and, or, not
The "and" and "or" operations are binary and require pre and post arguments
Eg. "3 and 4" as well as "1 or 7" are valid, but "3 or" and "and 3" are not
Unary negation of variables is permitted, but not for complex functions
Eg. "not 4" is valid, whereas "not (3 and 4)" is invalid
Evaluation precedence may be explicitly stated using the '(' and ')' characters
Parentheses may be nested, with no regard for whitespace or repeated characters
Eg. "((2 and 3) and 4)", "((1 and 2))" and "(7 or (1 or 3))" are all valid
An example valid input is "((2 and not 3) or not 4) and (5 or not 7 or (not 9 and 10))"
Please note, the length of each state is implicitly determined by the largest provided variable number
Any unused but implied variables will have all possible combinations calculated for logical consistency
As such, it is recommended that all input formula use the smallest variable numbers possible in order to reduce computational load
Eg. "1 and 3" would only create 2 possible states, but "1 and 101" would create 2^100 possible states

## Compilation:
To compile simply run the following commands:
```
cmake .
make
```

Compilation requires:
 - CMake 3.5.1 or later
 - POSIX Operating System
 - OpenMP 4.5 or later compliant compiler
 - C++17 compliant compiler

