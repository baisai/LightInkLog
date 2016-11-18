

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

#ifndef LIGHTINK_THREAD_ATOMIC_H_
#define LIGHTINK_THREAD_ATOMIC_H_

#include "Common/Type.h"
#include "Atomic/mintomic.h"

namespace LightInk
{
	template <typename T>
	class LIGHTINK_TEMPLATE_DECL Atomic;

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<uint32>
	{
	public:
		Atomic() { store(0); }
		inline uint32 load() { return mint_load_32_relaxed(&m_handle); }
		inline void store(uint32 desired) { mint_store_32_relaxed(&m_handle, desired); }
		inline uint32 exchange(uint32 desired) { return mint_exchange_32_relaxed(&m_handle, desired); }
		inline uint32 compare_exchange(uint32 expected, uint32 desired) { return mint_compare_exchange_strong_32_relaxed(&m_handle, expected, desired); }
		inline uint32 fetch_add(int32 operand) { return mint_fetch_add_32_relaxed(&m_handle, operand); }
		inline uint32 fetch_or(uint32 operand) { return mint_fetch_or_32_relaxed(&m_handle, operand); }
		inline uint32 fetch_and(uint32 operand) { return mint_fetch_and_32_relaxed(&m_handle, operand); }

	private:
		mint_atomic32_t m_handle;
	LIGHTINK_DISABLE_COPY(Atomic)
	};

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<uint64>
	{
	public:
		Atomic() { store(0); }
		inline uint64 load() { return mint_load_64_relaxed(&m_handle); }
		inline void store(uint64 desired) { mint_store_64_relaxed(&m_handle, desired); }
		inline uint64 exchange(uint64 desired) { return mint_exchange_64_relaxed(&m_handle, desired); }
		inline uint64 compare_exchange(uint64 expected, uint64 desired) { return mint_compare_exchange_strong_64_relaxed(&m_handle, expected, desired); }
		inline uint64 fetch_add(int64 operand) { return mint_fetch_add_64_relaxed(&m_handle, operand); }
		inline uint64 fetch_or(uint64 operand) { return mint_fetch_or_64_relaxed(&m_handle, operand); }
		inline uint64 fetch_and(uint64 operand) { return mint_fetch_and_64_relaxed(&m_handle, operand); }
	private:
		mint_atomic64_t m_handle;
	LIGHTINK_DISABLE_COPY(Atomic)
	};

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<void *>
	{
	public:
		Atomic() { store(NULL); }
		inline void * load() { return mint_load_ptr_relaxed(&m_handle); }
		inline void store(void * desired) { mint_store_ptr_relaxed(&m_handle, desired); }
		inline void * exchange(void * desired) { return mint_exchange_ptr_relaxed(&m_handle, desired); }
		inline void * compare_exchange(void * expected, void * desired) { return mint_compare_exchange_strong_ptr_relaxed(&m_handle, expected, desired); }
		inline void * fetch_add(ptrdiff_t operand) { return mint_fetch_add_ptr_relaxed(&m_handle, operand); }
		inline void * fetch_or(size_t operand) { return mint_fetch_or_ptr_relaxed(&m_handle, operand); }
		inline void * fetch_and(size_t operand) { return mint_fetch_and_ptr_relaxed(&m_handle, operand); }
	private:
		mint_atomicPtr_t m_handle;
	LIGHTINK_DISABLE_COPY(Atomic)
	};

}

#endif