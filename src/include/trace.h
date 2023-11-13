#pragma once

#include <stdio.h>

void dump_trace(char* prg_name);
int print_trace(FILE* fp, char* prg_name);
int get_trace(char* prg_name, char* buffer, size_t buffer_size);
