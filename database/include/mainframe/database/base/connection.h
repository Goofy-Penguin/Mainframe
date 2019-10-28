#pragma once

#include <string>
#include <memory>
#include <mainframe/database/base/database.h>

namespace mainframe {
	namespace database {
		class Connection {
			std::string serverIp;
			std::string serverUser;
			std::string serverPass;
			int serverPort = 0;

		public:
			virtual bool connect(const std::string& ip, int port, const std::string& user, const std::string& pass);
			virtual bool reconnect();
			virtual void disconnect() = 0;

			const std::string& getUser();
			const std::string& getPass();
			const std::string& getIp();
			int getPort();

			virtual const std::string& getLastError() const = 0;
			virtual std::vector<std::string> listDatabases() = 0;
			virtual std::shared_ptr<Database> getDatabase(const std::string& name) = 0;
		};
	}
}