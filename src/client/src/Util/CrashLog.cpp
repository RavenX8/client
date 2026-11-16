#include "StdAfx.h"
#include "CrashLog.h"

#include <windows.h>
#include <dbghelp.h>
#include <cstdio>
#include <cstring>

#pragma comment(lib, "dbghelp.lib")

static LONG WINAPI WriteCrashLog(EXCEPTION_POINTERS* ep) {
  FILE* f = nullptr;
  if ( fopen_s( &f, "crash.txt", "w" ) != 0 || !f )
    return EXCEPTION_CONTINUE_SEARCH;

  const EXCEPTION_RECORD* rec = ep->ExceptionRecord;
  fprintf( f, "exception 0x%08lX at %p\n", rec->ExceptionCode, rec->ExceptionAddress );
  if ( rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2 )
    fprintf( f, "%s address %p\n", rec->ExceptionInformation[0] ? "writing" : "reading",
             (void*)rec->ExceptionInformation[1] );

  HANDLE  process = GetCurrentProcess();
  HANDLE  thread  = GetCurrentThread();
  CONTEXT ctx     = *ep->ContextRecord;
  SymInitialize( process, nullptr, TRUE );

  STACKFRAME64 frame{};
  frame.AddrPC.Offset    = ctx.Rip;
  frame.AddrPC.Mode      = AddrModeFlat;
  frame.AddrFrame.Offset = ctx.Rbp;
  frame.AddrFrame.Mode   = AddrModeFlat;
  frame.AddrStack.Offset = ctx.Rsp;
  frame.AddrStack.Mode   = AddrModeFlat;

  for ( int i = 0; i < 64; ++i ) {
    if ( !StackWalk64( IMAGE_FILE_MACHINE_AMD64, process, thread, &frame, &ctx, nullptr,
                       SymFunctionTableAccess64, SymGetModuleBase64, nullptr ) )
      break;
    const DWORD64 pc = frame.AddrPC.Offset;
    if ( !pc )
      break;
    HMODULE module = nullptr;
    char    path[MAX_PATH] = "?";
    if ( GetModuleHandleExA( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                             GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                             (LPCSTR)pc, &module ) )
      GetModuleFileNameA( module, path, MAX_PATH );
    const char* name = strrchr( path, '\\' ) ? strrchr( path, '\\' ) + 1 : path;
    fprintf( f, "#%02d %s+0x%llx\n", i, name, (unsigned long long)(pc - (DWORD64)module) );
  }
  fclose( f );
  return EXCEPTION_CONTINUE_SEARCH;
}

void InstallCrashLog() {
  SetUnhandledExceptionFilter( WriteCrashLog );
}
