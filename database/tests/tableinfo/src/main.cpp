#include <mainframe/database/mysql/connection.h>
#include <fmt/printf.h>

using namespace mainframe::database;

int main() {
	const std::string ip = "127.0.0.1";
	const int port = 3306;
	const std::string user = "root";
	const std::string pass = "";

	ConnectionMysql con;
	if (!con.connect(ip, port, user, pass)) {
		fmt::print("failed to connect to database server {}:{} as {}, lastError: {}\n", ip, port, user, con.getLastError());
		return -1;
	}

	auto databases = con.listDatabases();
	if (databases.empty()) {
		fmt::print("failed to list databases, lastError: {}\n", con.getLastError());
		return -1;
	}

	auto db = con.getDatabase(databases.front());
	auto sqlDb = std::dynamic_pointer_cast<DatabaseMysql>(db);

	auto tables = sqlDb->listTables();
	if (tables.empty()) {
		fmt::print("failed to list tables (or database is empty), lastError: {}\n", con.getLastError());
		return -1;
	}

	auto& firstTable = sqlDb->getTable(tables.front());

	fmt::print("Table name: `{}`\n", firstTable.name);
	fmt::print("Table field count: `{}`\n", firstTable.fields.size());
	fmt::print("{:->{}}\n", "", 96);
	fmt::print("| {:<16} | {:<16} | {:<16} | {:<16} | {:<16} |\n", "name", "type", "default", "nullable", "is primary");
	for (auto& field : firstTable.fields) {
		fmt::print("| {:<16} | {:<16} | {:<16} | {:<16} | {:<16} |\n", field.name, field.type, field.defaultValue, field.nullable, field.primary);
	}
	fmt::print("{:->{}}\n", "", 96);

	fmt::print("Table key count: `{}`\n", firstTable.keys.size());
	fmt::print("{:->{}}\n", "", 93);
	fmt::print("| {:<32} | {:<16} | {:<16} | {:<16} |\n", "name", "source field", "target table", "target field");
	for (auto& key : firstTable.keys) {
		fmt::print("| {:<32} | {:<16} | {:<16} | {:<16} |\n", key.name, key.sourceFieldName, key.targetTableName, key.targetFieldName);
	}
	fmt::print("{:->{}}\n", "", 93);


	return 0;
}
