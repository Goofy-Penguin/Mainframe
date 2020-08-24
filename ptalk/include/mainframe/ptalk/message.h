#pragma once

#include <string>
#include <atomic>
#include <mainframe/utils/event.hpp>
#include <nlohmann/json.hpp>

namespace mainframe {
	namespace ptalk {
		class Message {
			uint32_t id = 0;
			std::string name;
			nlohmann::json data;
			bool cancelled = false;

			static uint32_t generateUniqueId();

		public:
			Message();
			Message(const std::string& name_, const nlohmann::json& data_);

			void setData(const nlohmann::json& data_);
			void setName(const std::string& name_);
			void setId(uint32_t id_);
			void cancel();

			uint32_t getId() const;
			const std::string& getName() const;
			const nlohmann::json& getData() const;

			bool isCancelled() const;
		};
	}
}