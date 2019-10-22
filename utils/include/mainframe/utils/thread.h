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
		namespace thread {
			void initAsync(int workers = 2);
			void shutdownAsync();

			void runOnMain(const std::function<void()>& func, bool isblocking = false, bool forcequeue = false);
			void runOnAsync(const std::function<void()>& func, bool isblocking = false, bool forcequeue = false);

			bool isMain();

			void runMain();
			void runAsync();

			unsigned long getId();

			void setName(const std::string& name);
			void setName(std::thread* thread, const std::string& name);
#ifdef _MSC_VER
			void setName(uint32_t threadId, const std::string& name);
#else
			void setName(pthread_t threadId, const std::string& name);
#endif

			void clearMain();
			void clearAsync();
		};
	}
}