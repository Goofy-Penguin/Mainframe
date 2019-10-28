#pragma once

#include <string>
#include <memory>
#include <mainframe/database/base/database.h>
#include <mainframe/database/mysql/communicator.h>

namespace mainframe {
	namespace database {
		class DatabaseMysql : public Database {
			std::shared_ptr<Communicator> communicator;

		public:
			virtual Result execute(const std::string& query) override;
			virtual std::vector<std::string> listTables() override;

			DatabaseMysql(const std::string& name, const std::shared_ptr<Communicator>& _communicator);
		};
	}
}