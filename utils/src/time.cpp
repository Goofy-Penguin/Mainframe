#define _CRT_SECURE_NO_WARNINGS

#include <mainframe/utils/time.h>
#include <chrono>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdarg>
#include <fmt/format.h>

#ifdef WIN32
#include <winsock.h>
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#endif

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif


namespace mainframe {
	namespace utils {
		namespace time {
			struct timezone {
				int  tz_minuteswest; /* minutes W of Greenwich */
				int  tz_dsttime;     /* type of dst correction */
			};

#ifdef _MSC_VER
			int gettimeofday(struct timeval* tv, struct timezone* tz) {
				FILETIME ft;
				unsigned __int64 tmpres = 0;
				static int tzflag = 0;

				if (nullptr != tv) {
					GetSystemTimeAsFileTime(&ft);

					tmpres |= ft.dwHighDateTime;
					tmpres <<= 32;
					tmpres |= ft.dwLowDateTime;

					tmpres /= 10;  /*convert into microseconds*/
					/*converting file time to unix epoch*/
					tmpres -= DELTA_EPOCH_IN_MICROSECS;
					tv->tv_sec = (long)(tmpres / 1000000UL);
					tv->tv_usec = (long)(tmpres % 1000000UL);
				}

				if (nullptr != tz) {
					if (!tzflag) {
						_tzset();
						tzflag++;
					}
					tz->tz_minuteswest = _timezone / 60;
					tz->tz_dsttime = _daylight;
				}

				return 0;
			}
#endif

			time_t starttime = 0;
			void resetTime() {
				struct timeval start;

				time_t mtime, seconds, useconds;

				gettimeofday(&start, nullptr);

				seconds = start.tv_sec;
				useconds = start.tv_usec;

				mtime = (time_t)(((seconds) * 1000 + useconds / 1000.0) + 0.5);

				starttime = mtime;
			}

			float getMS() {
				static bool inited = false;
				if (!inited) {
					resetTime();
					inited = true;
				}

				struct timeval start;

				time_t mtime, seconds, useconds;

				gettimeofday(&start, nullptr);

				seconds = start.tv_sec;
				useconds = start.tv_usec;

				mtime = (time_t)(((seconds) * 1000 + useconds / 1000.0) + 0.5);

				return (float)(mtime - starttime);
			}

			std::string getDateFormated(time_t unixtime, const std::string& format) {
				struct tm ltime;
				ltime = *localtime(&unixtime);

				char* str = new char[format.size() + 512];
				strftime(str, format.size() + 512, format.c_str(), &ltime);

				std::string ret(str);
				delete[] str;

				return ret;
			}

			std::string getDurationFormated(time_t seconds, Duration secondMode, Duration minuteMode, Duration hourMode, Duration dayMode, Duration weekMode, Duration monthMode, Duration yearMode) {
				tm* ltime = gmtime(&seconds);
				ltime->tm_year -= 70;
				ltime->tm_mday -= 1;

				std::string str;
				bool force = false;

				int week = (ltime->tm_mday - ltime->tm_wday - 1) / 7;

				if (yearMode == Duration::ALWAYS || ltime->tm_year > 0) force = true;
				if (force && yearMode != Duration::NEVER) str += fmt::format("%2d:", ltime->tm_year);

				if (monthMode == Duration::ALWAYS || ltime->tm_mon > 0) force = true;
				if (force && monthMode != Duration::NEVER) str += fmt::format("%2d:", ltime->tm_mon);

				if (weekMode == Duration::ALWAYS || week > 0) force = true;
				if (force && weekMode != Duration::NEVER) str += fmt::format("%2d:", week);

				if (dayMode == Duration::ALWAYS || ltime->tm_mday - 1 > 0) force = true;
				if (force && dayMode != Duration::NEVER) str += fmt::format("%2d:", ltime->tm_mday);

				if (hourMode == Duration::ALWAYS || ltime->tm_hour > 0) force = true;
				if (force && hourMode != Duration::NEVER) str += fmt::format("%2d:", ltime->tm_hour);

				if (minuteMode == Duration::ALWAYS || ltime->tm_min > 0) force = true;
				if (force && minuteMode != Duration::NEVER) str += fmt::format("%2d:", ltime->tm_min);

				if (secondMode == Duration::ALWAYS || ltime->tm_sec > 0) force = true;
				if (force && secondMode != Duration::NEVER) str += fmt::format("%2d:", ltime->tm_sec);

				return str;
			}
		}
	}
}