
#include "Common/WinSnprintf.h"


#if defined(_MSC_VER) && _MSC_VER < 1900

#include <stdio.h>
#include <stdarg.h>

/* Emulate snprintf() on MSVC<2015, _snprintf() doesn't zero-terminate the buffer
 * on overflow...
 */
int WinSnpirintf(char* buf, size_t len, const char* fmt, ...) {
  int n;
  va_list ap;
  va_start(ap, fmt);

  n = _vscprintf(fmt, ap);
  vsnprintf_s(buf, len, _TRUNCATE, fmt, ap);

  va_end(ap);
  return n;
}

#endif