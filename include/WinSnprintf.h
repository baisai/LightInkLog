

#ifndef LIGHTINK_COMMON_WINSNPRINTF_H_
#define LIGHTINK_COMMON_WINSNPRINTF_H_

#include <stddef.h>
#include "Common/Config.h"

#if defined(_MSC_VER) && _MSC_VER < 1900

	#ifdef __cplusplus
	extern "C" {
	#endif

		LIGHTINK_DECL int WinSnpirintf(char* buf, size_t len, const char* fmt, ...);

	#ifdef __cplusplus
	}
	#endif

	#ifndef snprintf
	#define snprintf WinSnpirintf
	#endif

#endif


#endif

