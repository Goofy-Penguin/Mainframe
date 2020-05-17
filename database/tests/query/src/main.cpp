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
	auto tables = db->listTables();

	auto ret = db->execute("SELECT * from `" + tables.front() + "`");

	if (ret.rows.empty()) {
		fmt::print("empty select returned. lastError: {}\n", con.getLastError());
		return -1;
	}

	for (auto& column : *ret.columns) {
		fmt::print("{:<24}, ", column);
	}
	fmt::print("\n");

	for (auto row : ret.rows) {
		for (auto val : row.values) {
			fmt::print("{:<24}, ", val.get<std::string>());
		}

		fmt::print("\n");
	}

	return 0;
}
