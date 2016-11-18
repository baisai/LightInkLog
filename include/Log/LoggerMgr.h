

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

#ifndef LIGHTINK_LOG_LOGGERMGR_H_
#define LIGHTINK_LOG_LOGGERMGR_H_

#include <map>
#include "Log/FixedLogger.h"
#include "fmt/fmt.h"
#include "Log/async/AsyncMsg.h"

namespace LightInk
{
	struct LogItem;
	class AsyncThread;
	class LIGHTINK_DECL LoggerMgr : public SmallObject
	{
	public:
		template <int32 Idx>
		inline bool init_fixed_logger(const LogOption & op) 
		{ 
			bool r = FixedLogger<Idx>::init(op); 
			if (r) 
			{ 
				Guard<LogLockAuto> l(m_lock);
				m_fixedLogger.insert(std::pair<int32, LogAutoPtr<SingletonBase>::type>(Idx, 
				LogAutoPtr<SingletonBase>::type(new FixedLogger<Idx>)));
			}
			return r;
		}
		template <int32 Idx>
		inline Logger * get_fixed_logger() { return FixedLogger<Idx>::instance(); }

		static LoggerMgr * get_instance() { static LoggerMgr s_lm; return &s_lm; }

		static bool init_global_logger(const LogOption & op) { return get_instance()->init_fixed_logger<1>(op); }
		static Logger * get_global_logger() { return get_instance()->get_fixed_logger<1>(); }

		LoggerMgr() : m_thread(NULL) {  }
		~LoggerMgr();

		RuntimeError init_async(uint32 size, AsyncMsg::AsyncOverflow aof);

		void release();

		RuntimeError register_logger(LoggerPtr log);
		RuntimeError register_logger(Logger * log);
		bool unregister_logger(const string & name);
		void clear_user_logger();

		LoggerPtr get_logger(const LogOption & op);
		
		RuntimeError flush(Logger * logger);

		RuntimeError channel(Logger * logger, LogItem & item);


	private:
		typedef map< int32, LogAutoPtr<SingletonBase>::type >::type::const_iterator FixedLoggerPtrMapIter;
		map< int32, LogAutoPtr<SingletonBase>::type >::type m_fixedLogger;
		map<string, LoggerPtr>::type m_userLogger;
		LogLockAuto m_lock;

		AsyncThread * m_thread;

	LIGHTINK_DISABLE_COPY(LoggerMgr)
	};
	
}


#define LightInkLog LightInk::LoggerMgr::get_instance()
#define LightInkLogCppInit(op) LightInk::LoggerMgr::init_global_logger(op)
#define LightInkLogCpp LightInk::LoggerMgr::get_global_logger()

#endif