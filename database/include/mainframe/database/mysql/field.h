#pragma once

#include <string>
#include <vector>

namespace mainframe {
	namespace database {
		struct MysqlField {
			std::string name;
			std::string type;
			bool nullable = false;
			bool primary = false;
			std::string defaultValue;
			std::string extra;
		};
	}
}
