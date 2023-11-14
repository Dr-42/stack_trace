# Trace
Simple stack trace for C programs
--------------------------------------------------------------------------
## Prerequisites:
1. Make sure you have addr2line installed on linux
--------------------------------------------------------------------------
## Usage:
0. #define DR42_TRACE_IMPLEMENTATION before including this file
STB style include
```c
#define DR42_TRACE_IMPLEMENTATION
#include "trace.h"
```
1. Call print_trace() to print the stack trace
2. Call fprint_trace(fp) to print the stack trace to a file
3. Call sprint_trace(buffer) to print the stack trace to a buffer
--------------------------------------------------------------------------
# Compilation stuff:
1. Compile with -g
2. Link with -ldl -rdynamic on linux
3. Compile with -g -gcodeview on windows
4. Link with "-l dbghelp -fuse-ld=lld -Wl,--pdb="

Note: I have only tested this with the clang compiler
