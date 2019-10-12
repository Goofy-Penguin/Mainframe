#include <mainframe/utils/thread.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace mainframe {
	namespace utils {
#ifdef _MSC_VER
		unsigned long thread::mainThreadID = 0;
		unsigned long thread::asyncThreadID = 0;
#else
		pthread_t thread::mainThreadID = 0;
		pthread_t thread::asyncThreadID = 0;
#endif

		ringbuffer<std::function<void()>> thread::callsToDoOnMainThread {64};
		ringbuffer<std::function<void()>> thread::callsToDoOnAsyncThread {64};
		bool thread::shuttingdown = false;

		static bool inited = false;
		void thread::initAsync(int workers) {
			if (inited) return;
			inited = true;

			mainThreadID = getId();

			while (workers-- > 0) {
				int i = workers;
				new std::thread([i]() {
					setName("mainframe:async_#" + std::to_string(i));

					while (!shuttingdown) {
						runAsync();
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
				});
			}
		}

		void thread::shutdownAsync() {
			clearMain();
			clearAsync();

			inited = false;
		}

		void thread::runOnMain(const std::function<void()>& func, bool isblocking, bool forcequeue) {
			if (isMain() && !forcequeue) {
				func();
				return;
			}

			if (!isblocking) {
				callsToDoOnMainThread.push(func);

				return;
			}

			bool isdone = false;
			callsToDoOnMainThread.push([&isdone, &func]() {
				func();
				isdone = true;
			});

			while (!isdone) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		void thread::runOnAsync(const std::function<void()>& func, bool isblocking, bool forcequeue) {
			if (!isMain() && !forcequeue) {
				func();
				return;
			}

			if (!isblocking) {
				callsToDoOnAsyncThread.push(func);

				return;
			}

			bool isdone = false;
			callsToDoOnAsyncThread.push([&isdone, &func]() {
				func();
				isdone = true;
			});

			while (!isdone) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		void thread::runMain() {
			while (!shuttingdown) {
				if (!callsToDoOnMainThread.available()) break;
				callsToDoOnMainThread.pop()();
			}
		}

		void thread::runAsync() {
			while (!shuttingdown) {
				if (!callsToDoOnAsyncThread.available()) break;
				callsToDoOnAsyncThread.pop()();
			}
		}

		void thread::clearMain() {
			callsToDoOnMainThread.clear();
		}

		void thread::clearAsync() {
			callsToDoOnAsyncThread.clear();
		}

		bool thread::isMain() {
			return mainThreadID == getId();
		}

		void thread::setName(const std::string& name) {
			setName(getId(), name);
		}

		unsigned long thread::getId() {
#ifdef _MSC_VER
			return GetCurrentThreadId();
#else
			return pthread_self();
#endif
		}

#ifdef _WIN32
		const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
		typedef struct tagTHREADNAME_INFO {
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		} THREADNAME_INFO;
#pragma pack(pop)

		void thread::setName(uint32_t threadId, const std::string& name) {
			THREADNAME_INFO info;
			info.dwType = 0x1000;
			info.szName = name.c_str();
			info.dwThreadID = threadId;
			info.dwFlags = 0;

			__try {
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)& info);
			} __except (EXCEPTION_EXECUTE_HANDLER) {
			}
		}

		void thread::setName(std::thread* thread, const std::string& name) {
			setName(GetThreadId(static_cast<HANDLE>(thread->native_handle())), name);
		}

#else
		void thread::setName(pthread_t threadId, const std::string& name) {
			pthread_setname_np(threadId, name.c_str());
		}

		void thread::setName(std::thread* thread, const std::string& name) {
			auto handle = thread->native_handle();
			pthread_setname_np(handle, name.c_str());
		}
#endif
	}
}