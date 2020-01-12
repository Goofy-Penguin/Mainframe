#pragma once

#include <string>
#include <mainframe/networking/socket.h>
#include <mainframe/database/base/result.h>
#include <fmt/format.h>

namespace mainframe {
	namespace database {
		class Database {
			std::string name;

		public:
			template<typename... CallbackArgs>
			Result execute(const std::string& query, CallbackArgs... args) {
				return execute(fmt::format(query, args...));
			}

			virtual Result execute(const std::string& query) = 0;
			virtual std::vector<std::string> listTables() = 0;

			const std::string& getName();
			Database(const std::string& _name);
		};
	}
}