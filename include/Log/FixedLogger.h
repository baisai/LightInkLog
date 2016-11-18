

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

#ifndef LIGHTINK_LOG_FIXEDLOGGER_H_
#define LIGHTINK_LOG_FIXEDLOGGER_H_

#include "Common/SingletonBase.h"
#include "Log/Logger.h"

namespace LightInk
{
	template <int32 Idx>
	class LIGHTINK_TEMPLATE_DECL FixedLogger : public SingletonBase, public SmallObject
	{
	private:
		FixedLogger() {  }
		static inline Logger * instance() { return m_logger; }
		static inline bool init(const LogOption & op) 
		{ 
			Guard<LogLockAuto> l(m_lock);
			if (m_logger) { return false; }
			m_logger = new Logger(op);
			m_loggerPtr.reset(m_logger);
			return true;
		}

		void destroy() 
		{ 
			Guard<LogLockAuto> l(m_lock);
			if (m_logger)
			{ 
				m_logger = NULL;
				m_loggerPtr.reset();
			}
		}
		
	private:
		static Logger * m_logger;
		static LogLockAuto m_lock;
		static LoggerPtr m_loggerPtr;

		friend class LoggerMgr;

	LIGHTINK_DISABLE_COPY(FixedLogger)
	};

	template <int Idx>
	Logger * FixedLogger<Idx>::m_logger = NULL;
	template <int Idx>
	LogLockAuto FixedLogger<Idx>::m_lock;
	template <int Idx>
	LoggerPtr FixedLogger<Idx>::m_loggerPtr;

}

#endif