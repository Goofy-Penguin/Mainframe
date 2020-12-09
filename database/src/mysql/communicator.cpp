#include <mainframe/database/mysql/communicator.h>
#include <mainframe/database/mysql/database.h>
#include <algorithm>
#include <cstring>

// better safe than sorry, bloody windows
#undef min
#undef max

namespace mainframe {
	namespace database {

		const std::string& Communicator::getLastError() {
			return lastError;
		}

		bool Communicator::fail(const std::string& err, const std::string& title) {
			lastError = title + ":" + err;
			return false;
		}

		bool Communicator::fail(const std::string& err) {
			lastError = err;
			return false;
		}

		bool Communicator::write(const std::string& query) {
			// Send sql query
			// Details at: http://forge.mysql.com/wiki/MySQL_Internals_ClientServer_Protocol#Command_Packet


			// size, header, query

			// header
			buffer[4] = 0x3;

			// copy query in
			uint32_t len = static_cast<uint32_t>(query.size()) + 1;
			std::memcpy(buffer.data() + 5, query.data(), len  + 1);
			std::memcpy(buffer.data(), &len, sizeof(len));

			// send it
			return sock.sendAll(buffer.data(), 4 + len);
		}

		Result Communicator::execute(const std::string& query, DatabaseMysql* db) {
			std::lock_guard<std::mutex> lockguard(lock);

			if (db != nullptr && db != lastDb) {
				lastDb = db;

				if (!write("USE " + db->getName())) return false;
				if (!read().success()) return false;
			}

			if (!write(query)) return false;
			auto ret = read();
			if (!ret.success()) return ret;

			// when inserting, mysql doesn't return the id, so we need to request that if needed
			const char checklow[] = "insert into";
			const char checkhigh[] = "INSERT INTO";
			std::string insertcheck = query.substr(0, sizeof(checklow) - 1);
			if (insertcheck == checklow || insertcheck == checkhigh) {
				if (!write("SELECT LAST_INSERT_ID()")) return false;
				ret = read();
			}

			return ret;
		}

		Value::Type Communicator::mysqlTypeToBase(MysqlValueType type) {
			Value::Type ret;

			switch (type) {
				case MysqlValueType::BIT:
				case MysqlValueType::SHORT:
				case MysqlValueType::TINY:
				case MysqlValueType::YEAR:
				case MysqlValueType::INT24:
					ret = Value::Type::Int;
					break;

				case MysqlValueType::LONG:
				case MysqlValueType::LONGLONG:
					ret = Value::Type::Long;
					break;

				case MysqlValueType::DOUBLE:
				case MysqlValueType::DECIMAL:
				case MysqlValueType::NEW_DECIMAL:
					ret = Value::Type::Double;
					break;

				case MysqlValueType::FLOAT:
					ret = Value::Type::Float;
					break;

				case MysqlValueType::STRING:
				case MysqlValueType::SET:
				case MysqlValueType::TIMESTAMP:
				case MysqlValueType::TINY_BLOB:
				case MysqlValueType::LONG_BLOB:
				case MysqlValueType::MEDIUM_BLOB:
				case MysqlValueType::BLOB:
				case MysqlValueType::DATE:
				case MysqlValueType::DATETIME:
				case MysqlValueType::NEWDATE:
				case MysqlValueType::VAR_STRING:
				case MysqlValueType::VARCHAR:
				case MysqlValueType::ENUM:
				case MysqlValueType::GEOMETRY:
				case MysqlValueType::TIME:
					ret = Value::Type::String;
					break;

				case MysqlValueType::Null:
				default:
					ret = Value::Type::None;
					break;
			}

			return ret;
		}

		Result Communicator::read() {
			// Parse and display record set
			// Details at: http://forge.mysql.com/wiki/MySQL_Internals_ClientServer_Protocol#Result_Set_Header_Packet
			// [ref] http://dev.mysql.com/doc/internals/en/overview.html#status-flags

			std::vector<char> txtv(65536, 0); // medium blob
			char* txt = txtv.data();

			char* p = reinterpret_cast<char*>(buffer.data());
			unsigned char typ[1000] = {0};
			int fields = 0, field = 0, value = 0, row = 0, exit = 0, rc = 0, i = 0, prevrow = -1;

			char& b3 = p[3]; // status low byte
			char& b4 = p[4]; // status high byte

			Result queryResult = true;

			while (true) {
				rc = 0;

				uint32_t len;
				if (!sock.receiveAll(reinterpret_cast<unsigned char*>(&len), 4)) return fail("connection lost");
				len &= 0xffffff;

				if (!sock.receiveAll(buffer.data(), len)) return fail("connection lost");

				// 0. For non query sql commands we get just single success or failure response
				if (!exit) {
					if (buffer.front() == 0) break; // success

					// error
					if (buffer.front() == 0xff) {
						uint16_t len;
						memcpy(&len, buffer.data() + 1, 2);

						auto str = reinterpret_cast<const char*>(buffer.data() + 4);
						auto endPtr = str;
						while (*(endPtr + 1) != 0x00) {
							endPtr++;
						}

						return fail({str, endPtr});
					}
				}

				// 1. first thing we receive is number of fields
				if (!fields) {
					memcpy(&fields, buffer.data(), len);
					field = fields;
					continue;
				}

				// 3. 5. after receiving last field info or row we get this EOF marker or more results exist
				if (buffer.front() == 0xfe && len < 9) {
					if (len == 5) {
						int status = (b4 << 8) | b3;
						if (status & 0x0008) { // SERVER_MORE_RESULTS_EXISTS
							//typedef void(*TOnSep)(void*);
							//if (onsep) ((TOnSep)onsep)(userdata);
							continue;
						}
					}

					if (exit++) break; else continue;        // EOF
				}

				// 4. after receiving all field infos we receive row field values. One row per Receive/Packet
				while (value) {
					*txt = 0; i = fields - value; size_t len = 1; unsigned char g = *(unsigned char*)p;

					// ~net_field_length() @ libmysql.c {
					switch (g) {
						case 0:
						case 251: g = 0;      break; // NULL_LENGTH
						default:
						case   1: g = 1;      break;
						case 252: g = 2, ++p; break;
						case 253: g = 3, ++p; break;
						case 254: g = 8, ++p; break;
					}
					// @todo: beware little/big endianess here!
					memcpy(&len, p, g); p += g;
					//}

					Value::Type type = mysqlTypeToBase(static_cast<MysqlValueType>(typ[i]));

					// @todo: beware little/big endianess here!
					if (g) {
						memcpy(txt, p, len);
					}

					txt[len] = 0;

					//typedef long(*TOnValue)(void*, char*, int, int, int);
					//if (onvalue) ret = ((TOnValue)onvalue)(userdata, txt, row, i, type);

					if (prevrow != row) {
						queryResult.rows.emplace_back(queryResult.columns);
						prevrow = row;
					}

					queryResult.rows.back().values.emplace_back(txt, type);

					p += len;
					if (!--value) {
						row++;
						value = fields;
						p = reinterpret_cast<char*>(buffer.data());

						break;
					}
				}

				// 2. Second info we get are field infos like name type etc. One field per Receive/Packet
				if (field) {
					i = fields - field;
					char* cat = p; p += 1 + *p; *cat++ = 0;
					char* db = p; p += 1 + *p; *db++ = 0;
					char* table = p; p += 1 + *p; *table++ = 0;
					char* table_ = p; p += 1 + *p; *table_++ = 0;
					char* name = p; p += 1 + *p; *name++ = 0;
					char* name_ = p; p += 1 + *p; *name_++ = 0; *p++ = 0;
					short charset = *(short*)p; p += 2;
					long  length = *(long*)p; p += 4;
					typ[i] = *(unsigned char*)p; p += 1;
					short flags = *(short*)p; p += 2;
					unsigned char digits = *(unsigned char*)p; p += 3;
					char* Default = p;

					auto type = static_cast<MysqlValueType>(typ[i]);
					switch (type) {
						case MysqlValueType::STRING:
						case MysqlValueType::VAR_STRING:
						case MysqlValueType::LONG:
						case MysqlValueType::LONGLONG:
						case MysqlValueType::TIMESTAMP:
						case MysqlValueType::DATETIME:
						case MysqlValueType::DATE:
						case MysqlValueType::FLOAT:
						case MysqlValueType::DOUBLE:
						case MysqlValueType::TINY:
						case MysqlValueType::BLOB:
						case MysqlValueType::NEW_DECIMAL:
							break;

						default:
							throw std::runtime_error("Warning: unsupported type " + std::to_string(static_cast<int>(type)) + " in column: " + name);
					}

					if (!--field)
						value = fields;

					p = reinterpret_cast<char*>(buffer.data());

					length = std::clamp(length * 3, 60L, 200L);

					///
					//typedef long(*TOnField)(void*, char*, int, int, int, int);
					//if (onfield) ((TOnField)onfield)(userdata, name, row, i, length, type);

					auto& last = queryResult.columns->emplace_back(name);
					std::transform(last.begin(), last.end(), last.begin(), [](char c) { return std::tolower(c); });

				}
			}

			return queryResult;
		}
	}
}