# Operating System Ex2

The task is made of 3 parts: files operations, dynamic libraries, and basic shell.

### Part A
Contains:
* Tool 1:
cmp- the tool will compare two files, and return “0” if they are equal, and “1” if not.
Supports -v flag for verbose output and the -i flag, for “ignore case”.

usage: cmp <file1> <file2> -v
output: equal

* Tool 2:
copy-  the tool will copy a file to another place and/or name.
The tool will return “0” on success, or “1” on failure.
Supports -v for verbose flag and -f flag for force, allows to overwrite the target file.
usage copy <file1> <file2> -v
output: success

### Part B
Usage : encode/decode <codec> <message>

output: encoded/decoded string

example: “encode codecA aaaBBB” will return “AAAbbb”

example: “decode codecB EEEddd” will return “BBBaaa”

### Part C
Our implementation for a shell program.

## How to run the program?
In order to compile all classes on gcc compiler:
* make all

In order to compile a specific class with gcc compiler:
* gcc -c fileName.c -o fileName.o

In order to execute class:
* ./fileName
