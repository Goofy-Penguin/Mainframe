#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <map>
#include <functional>

#include "ringbuffer.hpp"

#ifndef _MSC_VER
#include <sys/prctl.h>
#endif

namespace mainframe {
	namespace utils {
		class thread {
#ifdef _MSC_VER
			static unsigned long mainThreadID;
			static unsigned long asyncThreadID;
#else
			static pthread_t mainThreadID;
			static pthread_t asyncThreadID;
#endif

			static unsigned int callsToDoOnAsyncThreadIndex;
			static ringbuffer<std::function<void()>> callsToDoOnMainThread;
			static ringbuffer<std::function<void()>> callsToDoOnAsyncThread;
			static bool shuttingdown;

		public:
			static void initAsync(int workers = 2);
			static void shutdownAsync();

			static void runOnMain(const std::function<void()>& func, bool isblocking = false, bool forcequeue = false);
			static void runOnAsync(const std::function<void()>& func, bool isblocking = false, bool forcequeue = false);

			static bool isMain();

			static void runMain();
			static void runAsync();

			static unsigned long getId();

			static void setName(const std::string& name);
			static void setName(std::thread* thread, const std::string& name);
#ifdef _MSC_VER
			static void setName(uint32_t threadId, const std::string& name);
#else
			static void setName(pthread_t threadId, const std::string& name);
#endif

			static void clearMain();
			static void clearAsync();
		};
	}
}