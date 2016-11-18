

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

#ifndef LIGHTINK_LOG_FORMATHELPER_H_
#define LIGHTINK_LOG_FORMATHELPER_H_

#include "Common/Config.h"
#include <time.h>
#include "fmt/fmt.h"
#include "Log/LogConfig.h"

namespace LightInk
{
	struct LIGHTINK_DECL FormatHelper
	{
		static inline int level_index(LogLevel::LEVEL l) { int li = l; int index = 0; while (li >>= 1) ++index; return index; }
		static inline const char * am_pm(const tm & t) { return t.tm_hour >= 12 ? "PM" : "AM"; }
		static inline int to_12h(const tm & t) { return t.tm_hour > 12 ? t.tm_hour - 12 : t.tm_hour; }
		static inline const char * level_string(LogLevel::LEVEL l) { return level[level_index(l)]; }
		static inline const char * full_level_string(LogLevel::LEVEL l) { return full_level[level_index(l)]; }
		static inline fmt::MemoryWriter & pad_n_join(fmt::MemoryWriter & w, int v1, int v2, char sep)
		{ w << fmt::pad(v1, 2, '0') << sep << fmt::pad(v2, 2, '0'); return w; }
		static inline fmt::MemoryWriter & pad_n_join(fmt::MemoryWriter & w, int v1, int v2, int v3, char sep)
		{ w << fmt::pad(v1, 2, '0') << sep << fmt::pad(v2, 2, '0') << sep << fmt::pad(v3, 2, '0'); return w; }

		static const char * const wdays[];
		static const char * const full_wdays[];
		static const char * const months[];
		static const char * const full_months[];
		static const char * const level[];
		static const char * const full_level[];
		static const char * const endLine;
		static const size_t endLineSize;
		static const char * const file_line;

	};

}

#endif