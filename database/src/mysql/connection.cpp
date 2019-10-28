#include <mainframe/database/mysql/connection.h>
#include <cassert>
#include <cstring>

namespace mainframe {
	namespace database {
		ConnectionMysql::ConnectionMysql() {
			communicator = std::make_shared<Communicator>();
		}

		const std::string& ConnectionMysql::getLastError() const {
			return communicator->getLastError();
		}

		bool ConnectionMysql::connect(const std::string& ip, int port, const std::string& user, const std::string& pass) {
			Connection::connect(ip, port, user, pass);

			auto& buffer = communicator->buffer;
			auto& sock = communicator->sock;

			size_t bufferSize = 1 << 24;
			if (buffer.size() != bufferSize) {
				buffer.resize(bufferSize);
			}

			passBlob = blob(sha1::sha1(pass));

			std::lock_guard<std::mutex> lockGuard(communicator->lock);

			sock.close();
			sock.create();
			if (sock.connect(ip, static_cast<unsigned short>(port)) != 0) return communicator->fail("failed to connect");

			auto count = sock.receive(buffer.data(), buffer.size(), 0);
			auto msg = std::string(reinterpret_cast<char*>(buffer.data()) + 5);

			if (buffer[4] < 10) return communicator->fail(msg, "Need MySql > 4.1");

			// Read server auth challenge and calc response by making SHA1 hashes from it and password
			// [ref] http://dev.mysql.com/doc/internals/en/connection-phase.html#packet-Protocol::Handshake

			auto salt = buffer.data() + msg.size() + 10;
			std::memcpy(salt + 8, salt + 27, 13);
			std::vector<unsigned char> hash = get_mysql_hash(passBlob, salt);

			// Construct client auth response
			// [ref] http://forge.mysql.com/wiki/MySQL_Internals_ClientServer_Protocol#Client_Authentication_Packet

			auto d = buffer.data() + 4;
			uint32_t loginflag = static_cast<uint32_t>(
				Flags::FOUND_ROWS |
				Flags::PROTOCOL_41 |
				Flags::SECURE_CONNECTION |
				Flags::LONG_PASSWORD |
				Flags::MULTI_RESULTS
			);

			uint32_t buffersize = static_cast<uint32_t>(buffer.size());

			std::memcpy(d, &loginflag, sizeof(loginflag)); d += 4; // login flags
			std::memcpy(d, &buffersize, sizeof(buffersize)); d += 4; // buffersize
			*d = 8; d += 24; // utf8 charset

			// write user
			auto userbuff = reinterpret_cast<char*>(d);
			std::strcpy(userbuff, user.c_str()); d += 1 + user.size();

			// write XOR encrypt response
			*d = 20; d += 1;

			for (int i = 0; i < 20; i++)
				d[i] = hash[i] ^ 0;

			d += 22;

			// calc final packet size and id
			uint32_t sizeId = (d - buffer.data() - 4) | (1 << 24);
			std::memcpy(buffer.data(), &sizeId, sizeof(sizeId));

			// send login
			sock.send(buffer.data(), d - buffer.data());

			// in case of login failure server sends us an error text
			uint32_t len;
			sock.receive(reinterpret_cast<unsigned char*>(&len), 4);
			len &= (1 << 24) - 1;

			count = sock.receive(buffer.data(), len);
			if (count == -1 || *buffer.data()) {
				std::string msg = reinterpret_cast<char*>(buffer.data()) + 3;
				return communicator->fail(count == -1 ? "Timeout" : msg, "Login Failed");
			}

			return true;
		}

		void ConnectionMysql::disconnect() {

		}

		std::vector<std::string> ConnectionMysql::listDatabases() {
			auto result = communicator->execute("SHOW DATABASES", nullptr);
			if (!result.success()) return {};

			std::vector<std::string> ret;

			for (auto& db : result.rows) {
				ret.push_back(db[0]);
			}

			return ret;
		}

		std::shared_ptr<Database> ConnectionMysql::getDatabase(const std::string& name) {
			auto db = std::make_shared<DatabaseMysql>(name, this->communicator);
			return std::dynamic_pointer_cast<Database>(db);
		}

		std::vector<unsigned char> ConnectionMysql::get_mysql_hash(const std::vector<unsigned char>& password, const unsigned char salt[20]) {
			// [ref] http://dev.mysql.com/doc/internals/en/authentication-method.html#packet-Authentication::Native41
			// SHA1( password ) XOR SHA1( "20-bytes random data from server" <concat> SHA1( SHA1( password ) ) )

			std::vector<unsigned char> hash1 = password; // blob( SHA1( "password" ) );
			std::vector<unsigned char> hash2 = blob(sha1::sha1(hash1));

			unsigned char concat[40];
			for (size_t i = 0; i < 40; i++)
				concat[i] = (i < 20 ? salt[i] : hash2[i - 20]);

			std::vector<unsigned char> hash3 = blob(sha1::sha1(concat, 40));

			for (size_t i = 0; i < 20; ++i)
				hash3[i] ^= hash1[i];

			return hash3;
		}

		std::vector<unsigned char> ConnectionMysql::blob(const sha1::Hash& raw) {
			std::vector<unsigned char> blob;
			for (std::vector<sha1::Basetype>::const_iterator it = raw.begin(); it != raw.end(); ++it) {
				static int i = 1;
				static char* low = (char*)&i;
				static bool is_big = (*low ? false : true);
				if (is_big)
					for (unsigned int i = 0; i < sizeof(sha1::Basetype); ++i)
						blob.push_back(((*it) >> (i * 8)) & 0xff);
				else
					for (int i = (int)sizeof(sha1::Basetype) - 1; i >= 0; --i)
						blob.push_back(((*it) >> (i * 8)) & 0xff);
			}
			return blob;
		}
	}
}