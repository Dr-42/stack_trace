#pragma once

#include <stdio.h>

void init_trace(char* argv0);

void print_trace(void);
int fprint_trace(FILE* fp);
int sprint_trace(char* buffer);
