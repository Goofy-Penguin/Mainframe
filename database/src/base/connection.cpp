#include <mainframe/database/base/connection.h>

namespace mainframe {
	namespace database {
		bool Connection::connect(const std::string& ip, int port, const std::string& user, const std::string& pass) {
			serverIp = ip;
			serverPort = port;

			serverUser = user;
			serverPass = pass;

			return false;
		}

		bool Connection::reconnect() {
			disconnect();
			return connect(this->getIp(), this->getPort(), this->getUser(), this->getPass());
		}

		const std::string& Connection::getUser() {
			return serverUser;
		}

		const std::string& Connection::getPass() {
			return serverPass;
		}

		const std::string& Connection::getIp() {
			return serverIp;
		}

		int Connection::getPort() {
			return serverPort;
		}
	}
}