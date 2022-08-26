#pragma once

#include <string>
#include <vector>

namespace mainframe {
	namespace database {
		struct MysqlForeignKey {
			std::string name;
			std::string sourceFieldName;
			std::string sourceTableName;
			std::string targetFieldName;
			std::string targetTableName;
		};
	}
}
