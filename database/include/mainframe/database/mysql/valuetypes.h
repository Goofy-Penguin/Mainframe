#pragma once

namespace mainframe {
	namespace database {
		enum class MysqlValueType : int {
			NONE = 0,
			BIT = 16,
			BLOB = 252,
			DATE = 10,
			DATETIME = 12,
			DECIMAL = 0,
			DOUBLE = 5,
			ENUM = 247,
			FLOAT = 4,
			GEOMETRY = 255,
			INT24 = 9,
			LONG = 3,
			LONG_BLOB = 251,
			LONGLONG = 8,
			MEDIUM_BLOB = 250,
			NEW_DECIMAL = 246,
			NEWDATE = 14,
			Null = 6,
			SET = 248,
			SHORT = 2,
			STRING = 254,
			TIME = 11,
			TIMESTAMP = 7,
			TINY = 1,
			TINY_BLOB = 249,
			VAR_STRING = 253,
			VARCHAR = 15,
			YEAR = 13
		};
	}
}