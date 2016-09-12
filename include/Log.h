

#ifndef LIGHTINK_COMMON_LOG_H_
#define LIGHTINK_COMMON_LOG_H_
#include <cstdio>
#include "Common/LogMsg.h"

/////////////////////////////////
//系统日志
////////////////////////////////

////////////////////////////
//Trace
///////////////////////////
#ifdef LightInkNoTrace
#define LogTrace(fmt, ...)
#define LogTraceReturn(v) return v
#define LogTraceReturnVoid return

#else
#define LogTrace(fmt, ...) \
		char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
		snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
		LightInk::LogMsgTrace ___trace(__FILE__, __LINE__, ___mess)
#define LogTraceReturn(v) \
		do {___trace.set_line(__LINE__); return v; } while (0)
#define LogTraceReturnVoid \
		do {___trace.set_line(__LINE__); return; } while (0)

#endif

////////////////////////
//Debug
///////////////////////
#ifdef LightInkNoDebug
#define LogDebug(fmt, ...)
#else
#define LogDebug(fmt, ...) \
	do  \
	{ \
		char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
		snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
		LightInkLog->debug(__FILE__, __LINE__, ___mess); \
	} while (0) 

#endif

////////////////////////
//Message
///////////////////////
#ifdef LightInkNoMessage
#define LogMessage(fmt, ...)
#else
#define LogMessage(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->message(__FILE__, __LINE__, ___mess); \
	} while (0) 

#endif

////////////////////////
//Warning
///////////////////////
#ifdef LightInkNoWarning
#define LogWarning(fmt, ...)
#else
#define LogWarning(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->warning(__FILE__, __LINE__, ___mess); \
	} while (0) 

#endif


////////////////////////
//Error
///////////////////////
#ifdef LightInkNoError
#define LogError(fmt, ...)
#else
#define LogError(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->error(__FILE__, __LINE__, ___mess); \
	} while (0)

#endif



////////////////////////
//ScriptDebug
///////////////////////
#ifdef LightInkNoScriptDebug
#define LogScriptDebug(fmt, ...)
#else
#define LogScriptDebug(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->script_debug(__FILE__, __LINE__, ___mess); \
	} while (0)
#endif



////////////////////////
//ScriptMessage
///////////////////////
#ifdef LightInkNoScriptMessage
#define LogScriptMessage(fmt, ...)
#else
#define LogScriptMessage(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->script_message(__FILE__, __LINE__, ___mess); \
	} while (0)
#endif

////////////////////////
//ScriptWarning
///////////////////////
#ifdef LightInkNoScriptWarning
#define LogScriptWarning(fmt, ...)
#else
#define LogScriptWarning(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->script_warning(__FILE__, __LINE__, ___mess); \
	} while (0)
#endif


////////////////////////
//ScriptError
///////////////////////
#ifdef LightInkNoScriptError
#define LogScriptError(fmt, ...)
#else
#define LogScriptError(fmt, ...) \
	do  \
	{ \
	char ___mess[LightInk::LogMsg::CacheChar] = {0}; \
	snprintf(___mess, LightInk::LogMsg::CacheChar, fmt, ##__VA_ARGS__); \
	LightInkLog->script_error(__FILE__, __LINE__, ___mess); \
	} while (0)
#endif


/////////////////////////
//ScriptError and Jump
////////////////////////
#ifdef LightInkNoScriptErrorJump
#define LogScriptErrorJump(L, fmt, ...) LogScriptError(fmt, ##__VA_ARGS__)
#else
#define LogScriptErrorJump(L, fmt, ...) luaL_error(L, fmt, ##__VA_ARGS__)
#endif

#endif