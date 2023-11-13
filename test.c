#include <stdio.h>
#include <stdlib.h>
#define DR42_TRACE
#include "trace.h"

int foo(int val) {
    int c = 1 + 2;
    print_trace();
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
    init_trace(argv[0]);
    baz(4);
    return 0;
}
