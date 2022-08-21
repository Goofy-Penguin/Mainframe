#pragma once

#include <string>
#include <vector>

#include <mainframe/database/mysql/foreignkey.h>
#include <mainframe/database/mysql/field.h>

namespace mainframe {
	namespace database {
		struct MysqlTable {
			std::string name;
			std::vector<MysqlField> fields;
			std::vector<MysqlForeignKey> keys;
		};
	}
}
