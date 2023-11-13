#include "trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* prg_name;

void init_trace(char* argv0){
  prg_name = argv0;
}

void print_trace(){
  fprint_trace(stdout);
}

int fprint_trace(FILE* fp){
  char buffer[1024];
  int len = sprint_trace(buffer);
  if (len < 0) {
    return -1;
  }
  fprintf(fp, "%s\n", buffer);
  return len;
}

#ifdef __linux__
#define _GNU_SOURCE
#include <execinfo.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_STACK_FRAMES 64
void* buffer[MAX_STACK_FRAMES];

int sprint_trace(char* buff){
  if (prg_name == NULL) {
    fprintf(stderr, "Error: prg_name not set\n");
    fprintf(stderr, "Call init_trace(argv[0]) in main\n");
    return -1;
  }
  buff[0] = '\0';
  int size = backtrace(buffer, MAX_STACK_FRAMES);
  printf("Stack trace:\n");
  char tmp[1024];
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
	strncat(buff, line_pos, 1);
	line_pos++;
      }
      // Skip pwd
      line_pos += strlen(pwd);
      // Skip slash
      line_pos++;
      // Print rest of line
      sprintf(tmp, "%s", line_pos);
    } else {
      sprintf(tmp, "%s", line);
    }

    strcat(buff, tmp);
    // Stop at main
    if (strstr(line, "main at") != NULL) {
      break;
    }
  }
  buff[strlen(buff) - 1] = '\0';
  return strlen(buff);
}
#endif
#ifdef _WIN64
#include <windows.h>
#include <dbghelp.h>

int sprint_trace(char* buffer){
  unsigned int 	i;
  void         * stack[ 100 ];
  unsigned short frames;
  SYMBOL_INFO  * symbol;
  HANDLE       	process;

  process = GetCurrentProcess();

  SymInitialize( process, NULL, TRUE );

  frames               = CaptureStackBackTrace( 1, 100, stack, NULL ); // Start from index 1 to skip the current function (print_trace)
  symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
  symbol->MaxNameLen   = 255;
  symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

  IMAGEHLP_LINE64 line;
  line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

  for( i = 0; i < frames; i++ ){
    SymFromAddr( process, ( DWORD64 )( stack[ i ]  - 1), 0, symbol );
    // Get filename and line number from the address of the function call
    DWORD displacement;
    if (SymGetLineFromAddr64(process, (DWORD64)(stack[i] - 1), &displacement, &line)) {
      char filename[1024];
      // Remove cwd from filename
      GetCurrentDirectory(1024, filename);
      char* cwd_ptr = strstr(line.FileName, filename);
      if (cwd_ptr != NULL) {
	strcpy(filename, cwd_ptr + strlen(filename) + 1);
      }
      // Print the stack trace
      printf( "%s at %s:%lu\n", symbol->Name, filename, line.LineNumber );
    } else {
      // Unable to get source file information
      sprintf(buffer, "%s at (unknown)\n", symbol->Name);
    }
    if (strcmp(symbol->Name, "main") == 0) {
      // Stop printing the stack trace after reaching main
      break;
    }
  }
  free( symbol );
  buffer[strlen(buffer) - 1] = '\0';
  return strlen(buffer);
}
#endif
