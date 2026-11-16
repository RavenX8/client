/**
 * @file platform_types.h
 * @brief Cross-platform type definitions
 * @author Cross-platform port
 * @version 1.0
 */

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#ifdef _WIN32
  // Windows already has these types defined
  #include <windows.h>
#else
  // Linux type definitions to match Windows types
  #include <cstdint>
  
  // Basic Windows types
  typedef uint8_t   BYTE;
  typedef uint16_t  WORD;
  typedef uint32_t  DWORD;
  typedef uint64_t  QWORD;
  
  typedef int8_t    CHAR;
  typedef int16_t   SHORT;
  typedef int32_t   LONG;
  typedef int64_t   LONGLONG;
  
  typedef uint8_t   UCHAR;
  typedef uint16_t  USHORT;
  typedef uint32_t  ULONG;
  typedef uint64_t  ULONGLONG;
  
  typedef int       INT;
  typedef unsigned int UINT;

  typedef int       BOOL;

  // Windows message types
  typedef unsigned long WPARAM;
  typedef long          LPARAM;
  typedef long          LRESULT;

  // DirectX color type (32-bit ARGB)
  typedef uint32_t      D3DCOLOR;

  // Windows string types
  typedef char          TCHAR;

  // Windows large integer types
  typedef uint64_t      DWORDLONG;
  typedef void*     LPVOID;
  typedef void*     PVOID;
  typedef char*     LPSTR;
  typedef const char* LPCSTR;
  typedef wchar_t*  LPWSTR;
  typedef const wchar_t* LPCWSTR;
  
  // Handle types (just void pointers on Linux)
  typedef void*     HANDLE;
  typedef void*     HWND;
  typedef void*     HINSTANCE;
  typedef void*     HMODULE;
  typedef void*     HDC;
  typedef void*     HBITMAP;
  typedef void*     HFONT;
  typedef void*     HBRUSH;
  typedef void*     HPEN;
  typedef void*     HICON;
  typedef void*     HCURSOR;
  typedef void*     HMENU;
  
  // Constants
  #ifndef TRUE
    #define TRUE  1
  #endif
  #ifndef FALSE
    #define FALSE 0
  #endif
  
  #ifndef NULL
    #define NULL  0
  #endif
  
  // Common Windows constants
  #define MAX_PATH 260
  
  // Message box constants
  #define MB_OK                0x00000000L
  #define MB_OKCANCEL          0x00000001L
  #define MB_ABORTRETRYIGNORE  0x00000002L
  #define MB_YESNOCANCEL       0x00000003L
  #define MB_YESNO             0x00000004L
  #define MB_RETRYCANCEL       0x00000005L
  #define MB_ICONHAND          0x00000010L
  #define MB_ICONQUESTION      0x00000020L
  #define MB_ICONEXCLAMATION   0x00000030L
  #define MB_ICONASTERISK      0x00000040L
  #define MB_ICONWARNING       MB_ICONEXCLAMATION
  #define MB_ICONERROR         MB_ICONHAND
  #define MB_ICONINFORMATION   MB_ICONASTERISK
  #define MB_ICONSTOP          MB_ICONHAND
  
  // ShowWindow constants
  #define SW_HIDE              0
  #define SW_SHOWNORMAL        1
  #define SW_NORMAL            1
  #define SW_SHOWMINIMIZED     2
  #define SW_SHOWMAXIMIZED     3
  #define SW_MAXIMIZE          3
  #define SW_SHOWNOACTIVATE    4
  #define SW_SHOW              5
  #define SW_MINIMIZE          6
  #define SW_SHOWMINNOACTIVE   7
  #define SW_SHOWNA            8
  #define SW_RESTORE           9
  #define SW_SHOWDEFAULT       10
  #define SW_FORCEMINIMIZE     11
  #define SW_MAX               11
  
  // File attributes
  #define FILE_ATTRIBUTE_READONLY             0x00000001
  #define FILE_ATTRIBUTE_HIDDEN               0x00000002
  #define FILE_ATTRIBUTE_SYSTEM               0x00000004
  #define FILE_ATTRIBUTE_DIRECTORY            0x00000010
  #define FILE_ATTRIBUTE_ARCHIVE              0x00000020
  #define FILE_ATTRIBUTE_DEVICE               0x00000040
  #define FILE_ATTRIBUTE_NORMAL               0x00000080
  #define FILE_ATTRIBUTE_TEMPORARY            0x00000100
  #define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200
  #define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400
  #define FILE_ATTRIBUTE_COMPRESSED           0x00000800
  #define FILE_ATTRIBUTE_OFFLINE              0x00001000
  #define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000
  #define FILE_ATTRIBUTE_ENCRYPTED            0x00004000
  
  // Error codes
  #define ERROR_SUCCESS                    0L
  #define ERROR_FILE_NOT_FOUND             2L
  #define ERROR_PATH_NOT_FOUND             3L
  #define ERROR_ACCESS_DENIED              5L
  #define ERROR_INVALID_HANDLE             6L
  #define ERROR_NOT_ENOUGH_MEMORY          8L
  #define ERROR_INVALID_DATA               13L
  #define ERROR_OUTOFMEMORY                14L
  #define ERROR_INVALID_PARAMETER          87L
  #define ERROR_INSUFFICIENT_BUFFER        122L
  
  // RECT structure
  typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
  } RECT, *PRECT, *LPRECT;
  
  // POINT structure
  typedef struct tagPOINT {
    LONG x;
    LONG y;
  } POINT, *PPOINT, *LPPOINT;
  
  // SIZE structure
  typedef struct tagSIZE {
    LONG cx;
    LONG cy;
  } SIZE, *PSIZE, *LPSIZE;
  
  // FILETIME structure
  typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
  } FILETIME, *PFILETIME, *LPFILETIME;
  
  // SYSTEMTIME structure
  typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
  } SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

  // Windows utility functions
  #define ZeroMemory(ptr, size) memset(ptr, 0, size)
  #define _ASSERT(expr) assert(expr)

  // Windows utility macros
  #define LOWORD(l) ((WORD)(((DWORD)(l)) & 0xffff))
  #define HIWORD(l) ((WORD)((((DWORD)(l)) >> 16) & 0xffff))

  // DirectX color macros
  #define D3DCOLOR_RGBA(r,g,b,a) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

  // Windows multimedia functions
  #define timeGetTime() ((DWORD)(clock() * 1000 / CLOCKS_PER_SEC))

  // Include necessary headers for Linux equivalents
  #include <cstring>
  #include <cassert>
  #include <ctime>    // for clock()

#endif // _WIN32

#endif // PLATFORM_TYPES_H
