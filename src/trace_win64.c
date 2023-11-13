#ifdef _WIN64
#include <stdio.h>
#include <windows.h>
#include <dbghelp.h>

void print_trace(char* prg_name){
  (void)prg_name;
  unsigned int   i;
  void         * stack[ 100 ];
  unsigned short frames;
  SYMBOL_INFO  * symbol;
  HANDLE         process;

  process = GetCurrentProcess();

  SymInitialize( process, NULL, TRUE );

  frames               = CaptureStackBackTrace( 1, 100, stack, NULL ); // Start from index 1 to skip the current function (print_trace)
  symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
  symbol->MaxNameLen   = 255;
  symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

  IMAGEHLP_LINE64 line;
  line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

  for( i = 0; i < frames; i++ )
  {
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
      printf( "%s at (source information not available)\n", symbol->Name);
    }
    if (strcmp(symbol->Name, "main") == 0) {
      // Stop printing the stack trace after reaching main
      break;
    }
  }
  free( symbol );
}
#endif
