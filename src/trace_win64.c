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

  frames               = CaptureStackBackTrace( 0, 100, stack, NULL );
  symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
  symbol->MaxNameLen   = 255;
  symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

  for( i = 0; i < frames; i++ )
  {
    SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );
    // Get filename and line number from symbol->Address
    char filename[1024];
    DWORD displacement;
    IMAGEHLP_LINE64 line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    if (SymGetLineFromAddr64(process, symbol->Address, &displacement, &line)) {
      strcpy(filename, line.FileName);
    } else {
      strcpy(filename, "unknown");
    }
    // Remove cwd from filename
    char cwd[1024];
    GetCurrentDirectory(1024, cwd);
    char* cwd_ptr = strstr(filename, cwd);
    if (cwd_ptr != NULL) {
      strcpy(filename, cwd_ptr + strlen(cwd) + 1);
    }
    // Print the stack trace
    printf( "%s in %s:%lu\n", symbol->Name, filename, line.LineNumber );
    if (strcmp(symbol->Name, "main") == 0) {
      break;
    }
  }
  free( symbol );
}
#endif
