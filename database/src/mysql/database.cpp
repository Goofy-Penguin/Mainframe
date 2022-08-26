#include <mainframe/database/mysql/database.h>

namespace mainframe {
	namespace database {
		Result DatabaseMysql::execute(const std::string& query) {
			return communicator->execute(query, this);
		}

		void DatabaseMysql::fillTableInfos() {
			auto lst = listTables();
			tables.clear();

			for (auto& tableName : lst) {
				auto resultFields = communicator->execute(fmt::format("DESCRIBE `{}`", tableName), this);
				auto resultKeys = communicator->execute(fmt::format("SELECT table_name, column_name, constraint_name, referenced_table_name, referenced_column_name FROM information_schema.key_column_usage WHERE table_schema = '{}' AND table_name = '{}'", this->getName(), tableName), this);
				if (!resultFields.success() || !resultKeys.success()) continue;

				MysqlTable table;
				table.name = tableName;

				for (auto& fieldInfo : resultFields.rows) {
					MysqlField field;
					field.name = fieldInfo["field"].get<std::string>();
					field.type = fieldInfo["type"].get<std::string>();
					field.extra = fieldInfo["extra"].get<std::string>();
					field.defaultValue = fieldInfo["default"].get<std::string>();
					field.nullable = fieldInfo["null"].get<std::string>() == "YES";

					table.fields.push_back(field);
				}

				for (auto& keyInfo : resultKeys.rows) {
					MysqlForeignKey key;
					key.name = keyInfo["constraint_name"].get<std::string>();
					key.sourceFieldName = keyInfo["column_name"].get<std::string>();
					key.sourceTableName = keyInfo["table_name"].get<std::string>();

					if (key.name == "PRIMARY") {
						auto iter = std::find_if(table.fields.begin(), table.fields.end(), [&](const MysqlField& field) {
							return field.name == key.sourceFieldName;
						});

						if (iter == table.fields.end()) throw std::runtime_error("Primary field inside of table not found. This should not happen?");
						iter->primary = true;

						continue;
					}

					key.targetFieldName = keyInfo["referenced_column_name"].get<std::string>();
					key.targetTableName = keyInfo["referenced_table_name"].get<std::string>();

					table.keys.push_back(key);
				}

				tables.push_back(table);
			}
		}

		bool DatabaseMysql::hasLoadedTables() {
			return !tables.empty();
		}

		const MysqlTable& DatabaseMysql::getTable(const std::string& name) {
			if (tables.empty()) fillTableInfos();

			auto iter = std::find_if(tables.begin(), tables.end(), [&](const MysqlTable& tbl) {
				return tbl.name == name;
			});

			if (iter == tables.end()) throw std::runtime_error("Table not found");
			return *iter;
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