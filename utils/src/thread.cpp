#include <mainframe/utils/thread.h>

#ifdef _MSC_VER
#include <windows.h>
#include <locale>
#include <codecvt>
#endif

namespace mainframe {
	namespace utils {
		namespace thread {
#ifdef _MSC_VER
			unsigned long mainThreadID = 0;
			unsigned long asyncThreadID = 0;
#else
			pthread_t mainThreadID = 0;
			pthread_t asyncThreadID = 0;
#endif

			ringbuffer<std::function<void()>> callsToDoOnMainThread {64};
			ringbuffer<std::function<void()>> callsToDoOnAsyncThread {64};
			bool shuttingdown = false;

			bool inited = false;
			void initAsync(int workers) {
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

			void shutdownAsync() {
				clearMain();
				clearAsync();

				inited = false;
			}

			void runOnMain(const std::function<void()>& func, bool isblocking, bool forcequeue) {
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

			void runOnAsync(const std::function<void()>& func, bool isblocking, bool forcequeue) {
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

			void runMain() {
				while (!shuttingdown) {
					if (!callsToDoOnMainThread.available()) break;
					callsToDoOnMainThread.pop()();
				}
			}

			void runAsync() {
				while (!shuttingdown) {
					if (!callsToDoOnAsyncThread.available()) break;
					callsToDoOnAsyncThread.pop()();
				}
			}

			void clearMain() {
				callsToDoOnMainThread.clear();
			}

			void clearAsync() {
				callsToDoOnAsyncThread.clear();
			}

			bool isMain() {
				return mainThreadID == getId();
			}

			void setName(const std::string& name) {
				setName(getId(), name);
			}

			unsigned long getId() {
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

			// sub function due object unwinding with __try
			void _setName(uint32_t threadId, const std::string& name) {
				std::wstring wide;
				wide.assign(name.begin(), name.end());
				SetThreadDescription(GetCurrentThread(), wide.c_str());
			}

			void setName(uint32_t threadId, const std::string& name) {
				THREADNAME_INFO info;
				info.dwType = 0x1000;
				info.szName = name.c_str();
				info.dwThreadID = threadId;
				info.dwFlags = 0;

				__try {
					RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
				} __except (EXCEPTION_EXECUTE_HANDLER) {
				}

				_setName(threadId, name);
			}

			void setName(std::thread* thread, const std::string& name) {
				setName(GetThreadId(static_cast<HANDLE>(thread->native_handle())), name);
			}

#pragma warning( push )
#pragma warning( disable: 4996)
			std::string getName() {
				PWSTR wide;
				if (FAILED(GetThreadDescription(GetCurrentThread(), &wide))) {
					return "";
				}

				std::wstring retw = wide;
				LocalFree(wide);

				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
				return converter.to_bytes(retw);
			}
#pragma warning( pop )
#else
			void setName(pthread_t threadId, const std::string& name) {
				pthread_setname_np(threadId, name.c_str());
			}

			void setName(std::thread* thread, const std::string& name) {
				auto handle = thread->native_handle();
				pthread_setname_np(handle, name.c_str());
			}

			std::string getName() {
				char threadname[256] {0};

				pthread_getname_np(getId(), threadname, sizeof(threadname));

				return threadname;
			}
#endif
		}
	}
}