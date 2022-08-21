#pragma once

#include <string>
#include <memory>
#include <mainframe/database/base/database.h>
#include <mainframe/database/mysql/table.h>
#include <mainframe/database/mysql/communicator.h>

namespace mainframe {
	namespace database {
		class DatabaseMysql : public Database {
			std::shared_ptr<Communicator> communicator;
			std::vector<MysqlTable> tables;


		public:
			template<typename... CallbackArgs>
			Result execute(const std::string& query, CallbackArgs... args) {
				return execute(fmt::format(query, args...));
			}

			virtual Result execute(const std::string& query) override;
			virtual std::vector<std::string> listTables() override;

			const MysqlTable& getTable(const std::string& name);
			bool hasLoadedTables();
			void fillTableInfos();

			std::shared_ptr<Communicator> getCommunicator() const;
			DatabaseMysql(const std::string& name, const std::shared_ptr<Communicator>& _communicator);
		};
	}
}