#pragma once

#include <string>
#include <vector>

#include <mainframe/database/base/row.h>

namespace mainframe {
	namespace database {
		class Result {
			bool failed = true;

		public:
			std::vector<Row> rows;
			std::shared_ptr<std::vector<std::string>> columns = std::make_shared<std::vector<std::string>>();

			Result() = default;
			~Result() = default;

			Result(bool success);

			bool success();
		};
	}
}