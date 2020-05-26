﻿#pragma once

#include <functional>
#include <type_traits>
#include <algorithm>
#include <map>
#include <string>
#include <atomic>
#include <memory>

namespace mainframe {
	namespace utils {
		template<typename... CallbackArgs>
		class EventNamed {
			std::atomic<size_t> uniqueId = 0;

		public:
			using Func = std::function<void(CallbackArgs...)>;

			class EventScoped {
				EventNamed<CallbackArgs...>* map = nullptr;
				std::string name;

			public:
				EventScoped() = default;
				EventScoped(EventNamed<CallbackArgs...>* map_, const std::string& name_) : map(map_), name(name_) { };

				~EventScoped() {
					if (map == nullptr) return;
					map->remove(name);
					map = nullptr;
				}

				void call(CallbackArgs... args) {
					if (map == nullptr) return;

					auto cb = map->find(name);
					if (cb == map->end()) return;

					cb->second(args...);
				}
			};

			EventNamed() = default;
			~EventNamed() = default;

			std::shared_ptr<EventScoped> add(Func callback) {
				std::string name = "__" + std::to_string(uniqueId++) + "__";
				calls[name] = callback;

				return std::make_shared<EventScoped>(this, name);
			}

			void add(const std::string& name, Func callback) {
				calls[name] = callback;
			}

			Func& operator [](const std::string& name) { return calls[name]; }
			auto find(const std::string& name) { return calls.find(name); }
			auto begin() { return calls.begin(); }
			auto end() { return calls.end(); }

			size_t size() { return calls.size(); }

			void clear() {
				calls.clear();
			}

			bool remove(const std::string& name) {
				auto found = calls.find(name);
				if (found == calls.end()) return false;

				calls.erase(found);
				return true;
			}

			void operator()(CallbackArgs... args) {
				// copy it so we can += and -= inside the callback
				auto callsCopy = calls;
				for (const auto& pair : callsCopy) {
					pair.second(args...);
				}
			}

		private:
			std::map<std::string, Func> calls;
		};
	}
}
