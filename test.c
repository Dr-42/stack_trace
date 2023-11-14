#include <stdio.h>
#include <stdlib.h>
#define DR42_TRACE_IMPLEMENTATION
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

int main() {
    if (argc < 1) {
        return EXIT_FAILURE;
    }
    baz(4);
    return 0;
}
