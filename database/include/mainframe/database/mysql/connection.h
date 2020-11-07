#pragma once

#include <string>
#include <mutex>
#include <mainframe/networking/socket.h>
#include <mainframe/database/base/connection.h>
#include <mainframe/database/mysql/communicator.h>
#include <mainframe/database/mysql/database.h>
#include <mainframe/libs/sha1.h>

namespace mainframe {
	namespace database {
		class ConnectionMysql : public Connection {
		public:
			enum class Flags : unsigned {
				LONG_PASSWORD = 1,           /* New more secure passwords */
				FOUND_ROWS = 2,              /* Found instead of affected rows */
				LONG_FLAG = 4,               /* Get all column flags */
				CONNECT_WITH_DB = 8,         /* One can specify db on connect */
				NO_SCHEMA = 16,              /* Don't allow database.table.column */
				COMPRESS = 32,               /* Can use compression protocol */
				ODBC = 64,                   /* Odbc client */
				LOCAL_FILES = 128,           /* Can use LOAD DATA LOCAL */
				IGNORE_SPACE = 256,          /* Ignore spaces before '(' */
				PROTOCOL_41 = 512,           /* New 4.1 protocol */
				INTERACTIVE = 1024,          /* This is an interactive client */
				SSL = 2048,                  /* Switch to SSL after handshake */
				IGNORE_SIGPIPE = 4096,       /* IGNORE sigpipes */
				TRANSACTIONS = 8192,         /* Client knows about transactions */
				RESERVED = 16384,            /* Old flag for 4.1 protocol */
				SECURE_CONNECTION = 32768,   /* New 4.1 authentication */
				MULTI_STATEMENTS = 65536,    /* Enable/disable multi-stmt support */
				MULTI_RESULTS = 131072       /* Enable/disable multi-results */
				//  REMEMBER_OPTIONS = (((ulong) 1) << 31)

			};

		private:

			std::shared_ptr<Communicator> communicator;
			std::mutex lock;
			std::vector<unsigned char> passBlob;

			std::vector<unsigned char> get_mysql_hash(const std::vector<unsigned char>& password, const unsigned char salt[20]);
			std::vector<unsigned char> blob(const mainframe::sha1::Hash & raw);

		public:
			virtual bool connect(const std::string& ip, int port, const std::string& user, const std::string& pass) override;
			virtual void disconnect() override;

			virtual std::vector<std::string> listDatabases() override;
			virtual std::shared_ptr<Database> getDatabase(const std::string& name) override;
			std::shared_ptr<Communicator> getCommunicator() const;

			virtual const std::string& getLastError() const override;

			ConnectionMysql();
			~ConnectionMysql() = default;
		};

		inline ConnectionMysql::Flags operator|(ConnectionMysql::Flags a, ConnectionMysql::Flags b) { return static_cast<ConnectionMysql::Flags>(static_cast<int>(a) | static_cast<int>(b)); }
	}
}