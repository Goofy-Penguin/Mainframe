#pragma once

#include <string>
#include <vector>
#include <memory>

#include <mainframe/database/base/value.h>

namespace mainframe {
	namespace database {
		class Row {
			std::shared_ptr<std::vector<std::string>> columns;

		public:
			std::vector<Value> values;

			Row() = default;
			~Row() = default;

			Row(const std::shared_ptr<std::vector<std::string>>& _columns);

			bool has(const std::string& key) const;

			const Value& get(const std::string& key) const;
			const Value& get(size_t index) const;
			const Value& operator[](const std::string& key) const;
			const Value& operator[](size_t index) const;
		};
	}
}