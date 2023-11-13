#ifdef __linux__
#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_STACK_FRAMES 64
void* buffer[MAX_STACK_FRAMES];

#define READ 0
#define WRITE 1

void print_trace(char* prg_name) {
    int size = backtrace(buffer, MAX_STACK_FRAMES);
    printf("Stack trace:\n");
    for (int i = 1; i < size; i++) {
        // Execute addr2line and get prettified names
        char addr2line_cmd[512];
        sprintf(addr2line_cmd, "addr2line -p -f -e %s %p", prg_name, buffer[i] - 1);
        char line[512];
        FILE* addr2line = popen(addr2line_cmd, "r");
        fgets(line, 512, addr2line);
        pclose(addr2line);
        // Remove pwd from path
        char* pwd = getenv("PWD");
        char* pwd_pos = strstr(line, pwd);
        if (pwd_pos != NULL) {
            // Preserve text before pwd
            char* line_pos = line;
            char* pwd_pos = strstr(line, pwd);
            while (line_pos != pwd_pos) {
                printf("%c", *line_pos);
                line_pos++;
            }
            // Skip pwd
            line_pos += strlen(pwd);
            // Skip slash
            line_pos++;
            // Print rest of line
            printf("%s", line_pos);
        }
        // Stop at main
        if (strstr(line, "main at") != NULL) {
            break;
        }
    }
}
#endif
