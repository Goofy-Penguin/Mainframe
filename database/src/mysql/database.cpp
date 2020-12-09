#include <mainframe/database/mysql/database.h>

namespace mainframe {
	namespace database {
		Result DatabaseMysql::execute(const std::string& query) {
			return communicator->execute(query, this);
		}

		std::vector<std::string> DatabaseMysql::listTables() {
			auto result = communicator->execute("SHOW TABLES", this);
			if (!result.success()) return {};

			std::vector<std::string> ret;

			for (auto& db : result.rows) {
				ret.push_back(db[0]);
			}

			return ret;
		}

		std::shared_ptr<Communicator> DatabaseMysql::getCommunicator() const {
			return communicator;
		}

		DatabaseMysql::DatabaseMysql(const std::string& name, const std::shared_ptr<Communicator>& _communicator) : communicator(_communicator), Database(name) { }
	}
}