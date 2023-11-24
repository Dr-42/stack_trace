#include <stdio.h>
#include <stdlib.h>
#define DR42_TRACE_IMPLEMENTATION
#include "trace.h"

void* stack[100];
int size = 0;

int foo(int val) {
    int c = 1 + 2;
    //print_trace();
    size = get_intermediate_trace(stack, 100);
    return val + c;
}

int bar(int val) {
    return foo(val);
}

int baz(int val) {
    return bar(val);
}

int main() {
    baz(4);
    char* buffer = malloc(2000);
    memset(buffer, 0, 2000);
    sprint_intermediate_trace(stack, buffer, 0, size);
    printf("%s\n", buffer);
    return 0;
}
