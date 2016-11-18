

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

#ifndef LIGHTINK_COMMON_ASYNCQUEUE_H_
#define LIGHTINK_COMMON_ASYNCQUEUE_H_

#include "Atomic/Atomic.h"

namespace LightInk
{
	template <typename T>
	class AsyncQueue : SmallObject
	{
	public:
		AsyncQueue(uint32 size) : m_size(size), m_queue(NULL)
		{
			m_queue = (T **)malloc_user(size * sizeof(T *));
			for (uint32 i = 0; i < size; i++)
			{
				m_queue[i] = NULL;
			}
			m_readIdx.store(0);
			m_writeIdx.store(0);
		}

		~AsyncQueue() { free_user(m_queue, m_size * sizeof(T *)); m_queue = NULL; }

		bool push(T * t)
		{
			while (true)
			{
				uint32 writeIdx = m_writeIdx.load();
				uint32 nextWriteIdx = writeIdx + 1;
				if (real_index(nextWriteIdx) == real_index(m_readIdx.load()))
				{
					return false;
				}
				if (m_writeIdx.compare_exchange(writeIdx, nextWriteIdx) != writeIdx)
				{
					continue;
				}
				if (nextWriteIdx >= m_size)
				{
					m_writeIdx.compare_exchange(nextWriteIdx, nextWriteIdx % m_size);
				}
				m_queue[real_index(writeIdx)] = t;
				return true;
			}

		}
		T * pop()
		{
			while (true)
			{
				uint32 readIdx = m_readIdx.load();
				uint32 nextReadIdx = readIdx + 1;
				if (real_index(m_writeIdx.load()) == real_index(readIdx))
				{
					return NULL;
				}
				if (m_readIdx.compare_exchange(readIdx, nextReadIdx) != readIdx)
				{
					continue;
				}
				if (nextReadIdx >= m_size)
				{
					m_readIdx.compare_exchange(nextReadIdx, nextReadIdx % m_size);
				}
				return m_queue[real_index(readIdx)];
			}
		}

	private:
		inline uint32 real_index(uint32 idx) { return (idx % m_size); }

	private:
		T ** m_queue;
		const uint32 m_size;
		Atomic<uint32> m_readIdx;
		Atomic<uint32> m_writeIdx;
	LIGHTINK_DISABLE_COPY(AsyncQueue)
	};
}


#endif