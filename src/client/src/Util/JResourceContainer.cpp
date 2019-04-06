#include "StdAfx.h"
#include "JResourceContainer.h"

namespace JED
{
  unsigned long GetHashCode(const char* pString) {
    if ( pString == nullptr )
      return 0;

    unsigned long i, len;
    unsigned long ch;
    unsigned long result;

    len      = strlen( pString );
    result   = 5381;
    for ( i  = 0; i < len; i++ ) {
      ch     = (unsigned long)pString[i];
      result = ((result << 5) + result) + ch; // hash * 33 + ch
    }

    return result;
  }
}
