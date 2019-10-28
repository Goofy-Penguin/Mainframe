#pragma once

#include <string>
#include <mutex>
#include <mainframe/networking/socket.h>
#include <mainframe/database/base/result.h>
#include <mainframe/database/mysql/valuetypes.h>

namespace mainframe {
	namespace database {
		class DatabaseMysql;

		class Communicator {
			bool write(const std::string& query);
			Result read();

			DatabaseMysql* lastDb = nullptr;

			Value::Type mysqlTypeToBase(MysqlValueType type);

		public:
			std::vector<unsigned char> buffer; // mysql defines max buffersize as 16mb
			mainframe::networking::Socket sock;
			std::string lastError;
			std::mutex lock;

			Result execute(const std::string& query, DatabaseMysql* db);

			bool fail(const std::string& err);
			bool fail(const std::string& err, const std::string& title);

			const std::string& getLastError();
		};
	}
}