

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


#ifndef LIGHTINK_LOG_LOGGER_H_
#define LIGHTINK_LOG_LOGGER_H_

#include <iostream>
#include "Log/channel/LogChannelList.h"
#include "Log/LogFormat.h"

namespace LightInk
{
	class LogOption;
	class LIGHTINK_DECL Logger : public SmallObject
	{
	public:
		friend class LoggerMgr;

		Logger(const LogOption & op);
		Logger(const string & name, ChannelListPtr cl);
		Logger(const string & name, ChannelList::LogChannelPtr c);
		Logger(const string & name, ChannelListPtr cl, const string & format);
		Logger(const string & name, ChannelList::LogChannelPtr c, const string & format);
		Logger(const string & name, ChannelListPtr cl, LogFormatPtr format);
		Logger(const string & name, ChannelList::LogChannelPtr c, LogFormatPtr format);
		virtual ~Logger() {  }


		RuntimeError log(LogLevel::LEVEL level, const char * fmt, const fmt::ArgList & args);
		RuntimeError inline trace(const char * fmt, const fmt::ArgList & args){ return log(LogLevel::LogMsg_Trace, fmt, args); }
		RuntimeError inline debug(const char * fmt, const fmt::ArgList & args) { return log(LogLevel::LogMsg_Debug, fmt, args); }
		RuntimeError inline message(const char * fmt, const fmt::ArgList & args) { return log(LogLevel::LogMsg_Message, fmt, args); }
		RuntimeError inline warning(const char * fmt, const fmt::ArgList & args) { return log(LogLevel::LogMsg_Warning, fmt, args); }
		RuntimeError inline error(const char * fmt, const fmt::ArgList & args) { return log(LogLevel::LogMsg_Error, fmt, args); }
		RuntimeError inline fatal(const char * fmt, const fmt::ArgList & args) { return log(LogLevel::LogMsg_Fatal, fmt, args); }
		FMT_VARIADIC(RuntimeError, log, LogLevel::LEVEL, const char *)
		FMT_VARIADIC(RuntimeError, trace, const char *)
		FMT_VARIADIC(RuntimeError, debug, const char *)
		FMT_VARIADIC(RuntimeError, message, const char *)
		FMT_VARIADIC(RuntimeError, warning, const char *)
		FMT_VARIADIC(RuntimeError, error, const char *)
		FMT_VARIADIC(RuntimeError, fatal, const char *)


		RuntimeError log_fl(const LogFileLine & fl, LogLevel::LEVEL level, const char * fmt, const fmt::ArgList & args);
		RuntimeError inline trace_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args){ return log_fl(fl, LogLevel::LogMsg_Trace, fmt, args); }
		RuntimeError inline debug_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) { return log_fl(fl, LogLevel::LogMsg_Debug, fmt, args); }
		RuntimeError inline message_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) { return log_fl(fl, LogLevel::LogMsg_Message, fmt, args); }
		RuntimeError inline warning_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) { return log_fl(fl, LogLevel::LogMsg_Warning, fmt, args); }
		RuntimeError inline error_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) { return log_fl(fl, LogLevel::LogMsg_Error, fmt, args); }
		RuntimeError inline fatal_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) { return log_fl(fl, LogLevel::LogMsg_Fatal, fmt, args); }
		FMT_VARIADIC(RuntimeError, log_fl, const LogFileLine &, LogLevel::LEVEL, const char *)
		FMT_VARIADIC(RuntimeError, trace_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, debug_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, message_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, warning_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, error_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, fatal_fl, const LogFileLine &, const char *)


		template <typename T>
		RuntimeError log(LogLevel::LEVEL level, const T & t)
		{
			if (!should_log(level)) { return RE_Success; }
			try
			{
				LogItem item(&m_name, level);
				item.m_msg << t;
				return channel(item);
			}
			catch (const std::exception & e)
			{
				if (m_err) { m_err(e.what()); }
				else { default_error_handle(e.what()); }
				return RE_Log_LogFailed;
			}
			catch (...)
			{
				if (m_err) { m_err("unknown error!!!"); }
				else { default_error_handle("unknown error!!!"); }
				return RE_Log_LogFailed;
			}
		}
		template <typename T>
		RuntimeError inline trace(const T & t) { return log(LogLevel::LogMsg_Trace, t); }
		template <typename T>
		RuntimeError inline debug(const T & t) { return log(LogLevel::LogMsg_Debug, t); }
		template <typename T>
		RuntimeError inline message(const T & t) { return log(LogLevel::LogMsg_Message, t); }
		template <typename T>
		RuntimeError inline warning(const T & t) { return log(LogLevel::LogMsg_Warning, t); }
		template <typename T>
		RuntimeError inline error(const T & t) { return log(LogLevel::LogMsg_Error, t); }
		template <typename T>
		RuntimeError inline fatal(const T & t) { return log(LogLevel::LogMsg_Fatal, t); }



		template <typename T>
		RuntimeError log_fl(const LogFileLine & fl, LogLevel::LEVEL level, const T & t)
		{
			if (!should_log(level)) { return RE_Success; }
			try
			{
				LogItem item(&m_name, level);
				item.m_fl = fl;
				item.m_msg << t;
				return channel(item);
			}
			catch (const std::exception & e)
			{
				if (m_err) { m_err(e.what()); }
				else { default_error_handle(e.what()); }
				return RE_Log_LogFailed;
			}
			catch (...)
			{
				if (m_err) { m_err("unknown error!!!"); }
				else { default_error_handle("unknown error!!!"); }
				return RE_Log_LogFailed;
			}
		}
		template <typename T>
		RuntimeError inline trace_fl(const LogFileLine & fl,const T & t) { return log_fl(fl, LogLevel::LogMsg_Trace, t); }
		template <typename T>
		RuntimeError inline debug_fl(const LogFileLine & fl,const T & t) { return log_fl(fl, LogLevel::LogMsg_Debug, t); }
		template <typename T>
		RuntimeError inline message_fl(const LogFileLine & fl,const T & t) { return log_fl(fl, LogLevel::LogMsg_Message, t); }
		template <typename T>
		RuntimeError inline warning_fl(const LogFileLine & fl,const T & t) { return log_fl(fl, LogLevel::LogMsg_Warning, t); }
		template <typename T>
		RuntimeError inline error_fl(const LogFileLine & fl,const T & t) { return log_fl(fl, LogLevel::LogMsg_Error, t); }
		template <typename T>
		RuntimeError inline fatal_fl(const LogFileLine & fl,const T & t) { return log_fl(fl, LogLevel::LogMsg_Fatal, t); }

		RuntimeError flush();
		RuntimeError channel(LogItem & item);

		inline bool should_log(LogLevel::LEVEL level) const { return (level & m_level) != 0;}
		inline bool should_flush(LogLevel::LEVEL level) const { return (level & m_flushLevel) != 0;}

		inline void add_level(LogLevel::LEVEL level) { m_level |= level; }
		inline void remove_level(LogLevel::LEVEL level) { m_level &= (~level); }
		inline void reset_level(uint32 level) { m_level = level; }

		inline void add_flush_level(LogLevel::LEVEL level) { m_flushLevel |= level; }
		inline void remove_flush_level(LogLevel::LEVEL level) { m_flushLevel &= (~level); }
		inline void reset_flush_level(uint32 level) { m_flushLevel = level; }

		inline uint32 get_level() const { return m_level; }
		inline uint32 get_flush_level() const { return m_flushLevel; }

		inline const string & get_name() const { return m_name; }
		inline void set_error_handle(OsHelper::log_err_handle err) { m_err = err; }
		inline OsHelper::log_err_handle get_error_handle() { return m_err; }


	protected:
		virtual inline void default_error_handle(const string & msg) { std::cerr << msg.c_str() << std::endl; }

		inline virtual RuntimeError do_flush() { return m_channel->flush(); }
		virtual RuntimeError do_channel(LogItem & item);


	protected:
		const string m_name;
		LogFormatPtr m_format;
		uint32 m_level;
		uint32 m_flushLevel;
		OsHelper::log_err_handle m_err;
		ChannelListPtr m_channel;

	LIGHTINK_DISABLE_COPY(Logger)

	};

	typedef LogSharedPtrAuto<Logger>::type LoggerPtr;

}


#endif