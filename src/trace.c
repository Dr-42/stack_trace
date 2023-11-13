#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>
#include <string.h>

#define MAX_STACK_FRAMES 64
void* buffer[MAX_STACK_FRAMES];

void print_trace(char* prg_name) {
    int size = backtrace(buffer, MAX_STACK_FRAMES);
    printf("Stack trace:\n");
    for (int i = 1; i < size; i++) {
        // Execute addr2line and get prettified names
        int pipefd[2];
        pid_t cpid;
        pipe(pipefd);
        cpid = fork();
        if (cpid == 0) {
            // Child
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            char addr2line_cmd[512];
            sprintf(addr2line_cmd, "addr2line -p -f -e %s %p", prg_name, buffer[i] - 1);
            system(addr2line_cmd);
            exit(EXIT_SUCCESS);
        } else {
            // Parent
            close(pipefd[1]);
            char line[512];
            ssize_t bytes = read(pipefd[0], line, 512);
            line[bytes] = '\0';
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
            close(pipefd[0]);
            // Stop at main
            if (strstr(line, "main at") != NULL) {
                break;
            }
        }

    }
}
