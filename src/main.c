#include <stdio.h>
#include <stdlib.h>
#include "trace.h"

char* prg_name = NULL;
char buff[1024];

int foo(int val) {
    int c = 1 + 2;
    dump_trace(prg_name);
    return val + c;
}

int bar(int val) {
    return foo(val);
}

int baz(int val) {
    return bar(val);
}

int main(int argc, char** argv) {
    if (argc < 1) {
        return EXIT_FAILURE;
    }
    prg_name = argv[0];
    baz(4);
    return 0;
}
