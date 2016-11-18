

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

#ifndef LIGHTINK_LOG_LOGTHREADTYPE_H_
#define LIGHTINK_LOG_LOGTHREADTYPE_H_


#include "Common/TypeTool.h"
#include "Common/SharedPtr.h"
#include "Common/AutoPtr.h"
#include "Log/LogConfig.h"
#include "Atomic/mintomic.h"


#if defined(LIGHTINK_LOG_MULTITHREAD) && defined(LIGHTINK_LOG_CXX11)

#include <thread>
#include <mutex>

namespace LightInk
{
	typedef std::mutex Mutex;

	class LIGHTINK_DECL Thread : public SmallObject
	{
	public:
		Thread() : m_running(false), m_handle(NULL) {  }
		virtual ~Thread() 
		{ 
			if (m_handle)
			{
				m_handle->join();
				delete m_handle;
				m_handle = NULL;
			} 
		}

		int32 run()
		{
			if (m_running)
			{
				return 0;
			}
			m_running = true;
			m_handle = new std::thread(work_thread, this);
			return 0;
		}
		virtual void work() {  }

		bool is_running() { return m_running; }

		static void work_thread(void * arg)
		{
			Thread * t = (Thread *) arg;
			t->work();
			t->m_running = false;
		}

		static uint32 thread_self()
		{
			return std::this_thread::get_id().hash();
		}

	protected:
		bool m_running;
		std::thread * m_handle;

	LIGHTINK_DISABLE_COPY(Thread)
	};
}

#endif

namespace LightInk
{
#ifdef LIGHTINK_LOG_MULTITHREAD
	typedef Mutex LogLock;
	typedef Thread LogThread;
#endif

#define LogSleepMillis mint_sleep_millis
	class LIGHTINK_DECL LogEmptyLock
	{
	public:
		LogEmptyLock() {}
		~LogEmptyLock() {}
		void lock() {}
		int32 try_lock() { return 0; }
		void unlock() {}

	};

	template <typename T>
	struct LogSharedPtrTS
	{
		typedef SharedPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> type;
	};
	template <typename T>
	struct LogAutoPtr
	{
		typedef AutoPtrWrapper<T, PtrDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct LogSharedPtr
	{
		typedef SharedPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> type;
	};

#ifdef LIGHTINK_LOG_MULTITHREAD
#define LogSharedPtrAuto LightInk::LogSharedPtrTS
#define LogLockAuto LightInk::LogLock
#else
#define LogSharedPtrAuto LightInk::LogSharedPtr
#define LogLockAuto LightInk::LogEmptyLock
#endif
	
#define SelectSharedPtr(M, T) typename TypeSelect<IsSameType<M, LogEmptyLock>::Result, LogSharedPtr<T>::type, LogSharedPtrTS<T>::type>::Result

	
}




#endif