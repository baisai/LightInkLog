

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

#ifndef LIGHTINK_LOG_OSHELPER_H_
#define LIGHTINK_LOG_OSHELPER_H_

#include "Common/Type.h"
#include "Common/RuntimeError.h"
#include "Log/LogConfig.h"
#include "Common/STLType.h"

#include "stdio.h"
#include <string>
#include<time.h>
#include <string.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <sys/time.h>
#include <errno.h>
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <WinSock2.h>
#endif

namespace LightInk
{
	struct LIGHTINK_DECL OsHelper
	{
		typedef void (* log_err_handle)(const string & err);

#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
		typedef wstring FileNameType;
		typedef wchar_t FileCharType;
#else
		typedef string FileNameType;
		typedef char FileCharType;
#endif



		static inline tm localtime(const time_t &time_tt)
		{

#ifdef _WIN32
			tm t;
			::localtime_s(&t, &time_tt);
#else
			tm t;
			::localtime_r(&time_tt, &t);
#endif
			return t;
		}
		static inline tm localtime()
		{
			return localtime(time(NULL));
		}

		static inline tm gmtime(const time_t &time_tt)
		{

#ifdef _WIN32
			tm t;
			::gmtime_s(&t, &time_tt);
#else
			tm t;
			::gmtime_r(&time_tt, &t);
#endif
			return t;
		}
		static inline tm gmtime()
		{
			return gmtime(time(NULL));
		}


		static inline void gettimeofday(struct timeval * tv)
		{
#ifdef _WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define delta 11644473600000000Ui64
#   else
#define delta 11644473600000000ULL
#   endif
#define secOffSet 0.000001
#define usecOffSet 1000000
			if (tv != nullptr) 
			{
				FILETIME fileTime;
				GetSystemTimeAsFileTime(&fileTime);
				uint64 present = 0;
				present |= fileTime.dwHighDateTime;
				present <<= 32;
				present |= fileTime.dwLowDateTime;
				present /= 10;  // mic-sec
				// Subtract the difference
				present -= delta;
				tv->tv_sec = static_cast<long>(present * secOffSet);
				tv->tv_usec = static_cast<long>(present % usecOffSet);
			}
#undef delta
#undef secOffSet
#undef usecOffSet
#else
			::gettimeofday(tv, NULL);
#endif
		}


		static inline FILE* fopen(const FileNameType& filename, const FileCharType * mode)
		{
#ifdef _WIN32
#ifdef LIGHTINK_LOG_WCHAR_FILENAMES
			FILE* fp = ::_wfsopen((filename.c_str()), mode, _SH_DENYWR);
#else
			FILE* fp = ::_fsopen((filename.c_str()), mode, _SH_DENYWR);
#endif
#else
			FILE* fp = ::fopen((filename.c_str()), mode);
#endif
			return fp;
		}
		static inline int remove(const FileNameType &filename)
		{
#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
			return ::_wremove(filename.c_str());
#else
			return ::remove(filename.c_str());
#endif
		}
		static inline int rename(const FileNameType & oldName, const FileNameType & newName)
		{
#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
			return ::_wrename(oldName.c_str(), newName.c_str());
#else
			return ::rename(oldName.c_str(), newName.c_str());
#endif
		}
		static inline bool file_exists(const FileNameType & filename)
		{
#ifdef _WIN32
#ifdef LIGHTINK_LOG_WCHAR_FILENAMES
			uint32 attribs = ::GetFileAttributesW(filename.c_str());
#else
			uint32 attribs = ::GetFileAttributesA(filename.c_str());
#endif
			return (attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
#else //common linux/unix all have the stat system call
			struct stat buffer;
			return (::stat (filename.c_str(), &buffer) == 0);
#endif
		}
		static inline size_t file_size(FILE * fp)
		{
			if (fp == NULL) return 0;
#ifdef _WIN32
			int fd = ::_fileno(fp);
#if _WIN64 //64 bits
			struct _stat64 st;
			if (::_fstat64(fd, &st) == 0)
				return st.st_size;

#else //windows 32 bits
			struct _stat st;
			if (::_fstat(fd, &st) == 0)
				return st.st_size;
#endif

#else // unix
			int fd = ::fileno(fp);
			//64 bits(but not in osx, where fstat64 is deprecated)
#if !defined(__FreeBSD__) && !defined(__APPLE__) && (defined(__x86_64__) || defined(__ppc64__))
			struct stat64 st;
			if (::fstat64(fd, &st) == 0)
				return st.st_size;
#else // unix 32 bits or osx
			struct stat st;
			if (::fstat(fd, &st) == 0)
				return st.st_size;
#endif
#endif
			return 0;
		}
		static inline int utc_minutes_offset(const tm & t = localtime())
		{

#ifdef _WIN32
#if _WIN32_WINNT < _WIN32_WINNT_WS08
			TIME_ZONE_INFORMATION tzinfo;
			uint32 rv = GetTimeZoneInformation(&tzinfo);
#else
			DYNAMIC_TIME_ZONE_INFORMATION tzinfo;
			uint32 rv = GetDynamicTimeZoneInformation(&tzinfo);
#endif
			if (rv == TIME_ZONE_ID_INVALID) return 0;

			int offset = -tzinfo.Bias;
			if (t.tm_isdst)
				offset -= tzinfo.DaylightBias;
			else
				offset -= tzinfo.StandardBias;
			return offset;
#else

#if defined(sun) || defined(__sun)
			// 'tm_gmtoff' field is BSD extension and it's missing on SunOS/Solaris
			struct sun_helper
			{
				static long int calculate_gmt_offset(const tm & localtm, const tm & gmtm))
				{
					int local_year = localtm.tm_year + (1900 - 1);
					int gmt_year = gmtm.tm_year + (1900 - 1);

					long int days = (
										// difference in day of year
										localtm.tm_yday - gmtm.tm_yday

										// + intervening leap days
										+ ((local_year >> 2) - (gmt_year >> 2))
										- (local_year / 100 - gmt_year / 100)
										+ ((local_year / 100 >> 2) - (gmt_year / 100 >> 2))

										// + difference in years * 365 */
										+ (long int)(local_year - gmt_year) * 365
									);

					long int hours = (24 * days) + (localtm.tm_hour - gmtm.tm_hour);
					long int mins = (60 * hours) + (localtm.tm_min - gmtm.tm_min);
					long int secs = (60 * mins) + (localtm.tm_sec - gmtm.tm_sec);

					return secs;
				}
			};

			long int offset_seconds = sun_helper::calculate_gmt_offset(t, gmtime(mktime(t)));
#else
			long int offset_seconds = t.tm_gmtoff;
#endif

			return static_cast<int>(offset_seconds / 60);
#endif
		}

		static inline string filename_to_string(const FileNameType & filename)
		{
#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
			int destLen = WideCharToMultiByte( CP_OEMCP, NULL , filename.c_str(), filename.length(), NULL , NULL, NULL, NULL );
			char * pDest = new char[destLen +1];
			WideCharToMultiByte(CP_OEMCP , NULL, filename.c_str(), filename.length() , pDest, destLen, NULL , NULL);
			pDest[destLen] = '\0';
			string dest( pDest);
			delete [] pDest;
			return dest;
#else
			return filename;
#endif
		}

		static inline FileNameType string_to_filename(const string & str)
		{
#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
			int destLen = MultiByteToWideChar( CP_ACP, NULL , str.c_str(), -1, NULL, NULL);
			wchar_t * pDest = new wchar_t[destLen+1];
			MultiByteToWideChar(CP_ACP , NULL, str.c_str(), -1, pDest, destLen);
			pDest[destLen] = L'\0';
			wstring dest( pDest);
			delete [] pDest;
			return dest;
#else
			return str;
#endif
		}

		inline string errno_str(int err_num)
		{
			char buf[256];
			size_t buf_size = sizeof(buf);

#ifdef _WIN32
			if(::strerror_s(buf, buf_size, err_num) == 0)
				return string(buf);
			else
				return string("Unkown error");

#elif defined(__FreeBSD__) || defined(__APPLE__) || defined(__ANDROID__) || \
			  ((_POSIX_C_SOURCE >= 200112L) && ! _GNU_SOURCE) // posix version

			if (::strerror_r(err_num, buf, buf_size) == 0)
				return string(buf);
			else
				return string("Unkown error");

#else  // gnu version (might not use the given buf, so its retval pointer must be used)
			return string(::strerror_r(err_num, buf, buf_size));
#endif
		}

	};

#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
#define LIGHTINK_LOG_FILENAME_T(s) L ## s
#else
#define LIGHTINK_LOG_FILENAME_T(s) s
#endif


	inline bool operator==(const tm& tm1, const tm& tm2)
	{
		return (tm1.tm_sec == tm2.tm_sec &&
				tm1.tm_min == tm2.tm_min &&
				tm1.tm_hour == tm2.tm_hour &&
				tm1.tm_mday == tm2.tm_mday &&
				tm1.tm_mon == tm2.tm_mon &&
				tm1.tm_year == tm2.tm_year &&
				tm1.tm_isdst == tm2.tm_isdst);
	}
	inline bool operator!=(const tm& tm1, const tm& tm2)
	{
		return !(tm1 == tm2);
	}

}

#endif