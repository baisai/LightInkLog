


/* Copyright ChenDong(Wilbur), email <baisaichen@live.com>. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef LIGHTINK_COMMON_LOG_H_
#define LIGHTINK_COMMON_LOG_H_

#include "Common/Config.h"
#include "Log/LoggerMgr.h"
#include "Log/helper/LogTraceHelper.h"

/////////////////////////////////
//系统日志
////////////////////////////////

////////////////////////////
//Trace
///////////////////////////
#ifdef LightInkNoTrace
#define LogTrace(ft, ...)
#define LogTraceReturn(v) return v
#define LogTraceReturnVoid return
#define LogTraceEnd

#else
#define LogTrace(ft, ...) \
		fmt::MemoryWriter ___mess; \
		___mess.write(ft, ##__VA_ARGS__); \
		LightInk::LogTraceHelper ___trace(__FILE__, __LINE__, &___mess)
#define LogTraceReturn(v) \
		do {___trace.set_line(__LINE__); return v; } while (0)
#define LogTraceReturnVoid \
		do {___trace.set_line(__LINE__); return; } while (0)
#define LogTraceEnd \
		do {___trace.set_line(__LINE__); } while (0)
#endif

////////////////////////
//Debug
///////////////////////
#ifdef LightInkNoDebug
#define LogDebug(ft, ...)
#else
#define LogDebug(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->debug_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0) 

#endif

////////////////////////
//Message
///////////////////////
#ifdef LightInkNoMessage
#define LogMessage(ft, ...)
#else
#define LogMessage(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->message_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0) 

#endif

////////////////////////
//Warning
///////////////////////
#ifdef LightInkNoWarning
#define LogWarning(ft, ...)
#else
#define LogWarning(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->warning_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0) 

#endif


////////////////////////
//Error
///////////////////////
#ifdef LightInkNoError
#define LogError(ft, ...)
#else
#define LogError(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->error_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)

#endif

////////////////////////
//Fatal
///////////////////////
#ifdef LightInkNoFatal
#define LogFatal(ft, ...)
#else
#define LogFatal(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->fatal_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)

#endif

#endif