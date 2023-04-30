# Operating System Ex2

Compile the shared libraries codecA and codecB:

gcc -shared -fPIC -o codecA.so codecA.c
gcc -shared -fPIC -o codecB.so codecB.c

Compile the encode and decode tools using the shared libraries:
gcc -o encode encode.c -ldl
gcc -o decode decode.c -ldl

("-ldl" flag is used to link the tools against the dynamic loading library)

Make sure that all four files (codecA.so, codecB.so, encode, and decode) are in the same directory.
Here's an example of how to encode a message using codecA and the encode tool:

./encode codecA "aaaBBB"
./decode codecA "AAAbbb"

## How to run?
In order to compile all classes on gcc compiler:
* make all

In order to compile a specific class with gcc compiler:
* gcc -c fileName.c -o fileName.o

In order to execute class:
* ./fileName
