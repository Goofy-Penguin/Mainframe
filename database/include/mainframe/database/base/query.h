#pragma once

#include <string>

namespace mainframe {
	namespace database {
		class Query {
			std::string str;

		public:
			Query() = default;
			Query(const std::string& str);
		};
	}
}