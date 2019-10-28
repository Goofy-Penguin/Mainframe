#include <mainframe/database/base/database.h>

namespace mainframe {
	namespace database {
		const std::string& Database::getName() {
			return name;
		}

		Database::Database(const std::string& _name) : name(_name) {
		}
	}
}